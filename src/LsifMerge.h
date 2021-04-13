#include "clang-tools-extra/clangd/index/Index.h"

namespace clang {
namespace lsif {
clangd::Symbol mergeSymbol(
        const clangd::Symbol &L,
        const clangd::Symbol &R,
        const std::string &ProjectRoot);
}
}
