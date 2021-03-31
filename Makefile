default: cmake
	cmake --build build --target lsif-clang

cmake:
	mkdir -p build
	cmake -B build --target lsif-clang -S . \
		  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		  -DPATH_TO_LLVM=${PATH_TO_LLVM} \
		  -DLLVM_REPO_DIR=${LLVM_REPO_DIR}
