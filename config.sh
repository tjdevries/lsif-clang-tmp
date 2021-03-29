#!/bin/bash

set -e
set -x

cmake -B build --target lsif-clang -S . \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DPATH_TO_LLVM=$PATH_TO_LLVM \
      -DLLVM_REPO_DIR=$LLVM_REPO_DIR

if [[ ! -f "compile_commands.json" ]]; then
    ln -s $(pwd)/build/compile_commands.json ./
fi
