//
// Mostly taken from clang-tools-extra/clangd/index/Merge.cpp
//
// Primarily overriding `prefer`, but it is not exposed in the namespace,
// so we cannot just override it (at least to my knowledge...)
//

#include "LsifMerge.h"

namespace clang {
namespace lsif {

bool prefer(const clangd::SymbolLocation &L,
            const clangd::SymbolLocation &R,
            // Sourcegraph: Added ProjectRoot
            const std::string &ProjectRoot) {
  if (!L)
    return false;
  if (!R)
    return true;
  auto HasCodeGenSuffix = [](const clangd::SymbolLocation &Loc) {
    constexpr static const char *CodegenSuffixes[] = {".proto"};
    return std::any_of(std::begin(CodegenSuffixes), std::end(CodegenSuffixes),
                       [&](llvm::StringRef Suffix) {
                         return llvm::StringRef(Loc.FileURI).endswith(Suffix);
                       });
  };
  auto InProject = [ProjectRoot](const clangd::SymbolLocation &Loc) {
    return Loc.FileURI && llvm::StringRef(Loc.FileURI).startswith(ProjectRoot);
  };

  return (HasCodeGenSuffix(L) && !HasCodeGenSuffix(R))
      // Sourcegraph: Effectively, our only difference is that we have between our and clangd `prefer`
      || (InProject(L) && !InProject(R));
}

clangd::Symbol mergeSymbol(
        const clangd::Symbol &L,
        const clangd::Symbol &R,
        // Sourcegraph: We added `ProjectRoot` so that we can pass to `prefer`
        const std::string &ProjectRoot)
{
  assert(L.ID == R.ID);
  // We prefer information from TUs that saw the definition.
  // Classes: this is the def itself. Functions: hopefully the header decl.
  // If both did (or both didn't), continue to prefer L over R.
  bool PreferR = R.Definition && !L.Definition;
  // Merge include headers only if both have definitions or both have no
  // definition; otherwise, only accumulate references of common includes.
  assert(L.Definition.FileURI && R.Definition.FileURI);
  bool MergeIncludes =
      bool(*L.Definition.FileURI) == bool(*R.Definition.FileURI);
  clangd::Symbol S = PreferR ? R : L;        // The target symbol we're merging into.
  const clangd::Symbol &O = PreferR ? L : R; // The "other" less-preferred symbol.

  // Only use locations in \p O if it's (strictly) preferred.
  if (prefer(O.CanonicalDeclaration, S.CanonicalDeclaration, ProjectRoot))
    S.CanonicalDeclaration = O.CanonicalDeclaration;
  if (prefer(O.Definition, S.Definition, ProjectRoot))
    S.Definition = O.Definition;
  S.References += O.References;
  if (S.Signature == "")
    S.Signature = O.Signature;
  if (S.CompletionSnippetSuffix == "")
    S.CompletionSnippetSuffix = O.CompletionSnippetSuffix;
  if (S.Documentation == "") {
    // Don't accept documentation from bare forward class declarations, if there
    // is a definition and it didn't provide one. S is often an undocumented
    // class, and O is a non-canonical forward decl preceded by an irrelevant
    // comment.
    bool IsClass = S.SymInfo.Kind == index::SymbolKind::Class ||
                   S.SymInfo.Kind == index::SymbolKind::Struct ||
                   S.SymInfo.Kind == index::SymbolKind::Union;
    if (!IsClass || !S.Definition)
      S.Documentation = O.Documentation;
  }
  if (S.ReturnType == "")
    S.ReturnType = O.ReturnType;
  if (S.Type == "")
    S.Type = O.Type;
  for (const auto &OI : O.IncludeHeaders) {
    bool Found = false;
    for (auto &SI : S.IncludeHeaders) {
      if (SI.IncludeHeader == OI.IncludeHeader) {
        Found = true;
        SI.References += OI.References;
        break;
      }
    }
    if (!Found && MergeIncludes)
      S.IncludeHeaders.emplace_back(OI.IncludeHeader, OI.References);
  }

  S.Origin |= O.Origin | clangd::SymbolOrigin::Merge;
  S.Flags |= O.Flags;
  return S;
}

}
}
