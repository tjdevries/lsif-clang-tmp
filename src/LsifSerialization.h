//===-- LSIFSerialization.h ------------------------------------*- C++-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// LSIF spec:
// https://microsoft.github.io/language-server-protocol/specifications/lsif/0.5.0/specification/
//
//===----------------------------------------------------------------------===//

#include "clang-tools-extra/clangd/Headers.h"
#include "clang-tools-extra/clangd/index/Index.h"
#include "clang-tools-extra/clangd/index/Serialization.h"
#include "clang-tools-extra/clangd/index/Symbol.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang-tools-extra/clangd/index/Serialization.h"


namespace clang {
    namespace clangd {
        struct LsifIndexFile : clang::clangd::IndexFileOut {
            std::string ProjectRoot = "";
            bool Debug = false;
            bool DebugFiles = false;

            LsifIndexFile() = default;
            LsifIndexFile(const IndexFileIn &I, std::string project_root) : IndexFileOut(I) {
                ProjectRoot = project_root;
            }

        };

        void writeLSIF(const LsifIndexFile &O, llvm::raw_ostream &OS);
    }// namespace clangd
}// namespace clang
