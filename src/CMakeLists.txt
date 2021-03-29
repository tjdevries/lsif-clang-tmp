add_executable(lsif-clang LsifMain.cpp ${BACKWARD_ENABLE})

install(TARGETS lsif-clang)


# LLVM Dependencies
# target_include_directories(lsif-clang PUBLIC "/home/tjdevries/.cache/lsif-clang/llvm-project/")
# target_include_directories(lsif-clang PUBLIC "/home/tjdevries/.cache/lsif-clang/llvm-project/clang-tools-extra/clangd/")
target_link_libraries(lsif-clang stdc++)
target_link_libraries(lsif-clang clangDaemon)

# Backwards CPP Installation.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/backward-cpp)

add_backward(lsif-clang)
