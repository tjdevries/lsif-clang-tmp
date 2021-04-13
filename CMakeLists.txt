cmake_policy(SET CMP0048 NEW)

project(ClangLSIF VERSION 0.1)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -DNDEBUG -g")
set(LLVM_ENABLE_PROJECTS "clang;clang-tools-extra")

if(NOT LLVM_REPO_DIR)
  set(LLVM_REPO_DIR "/home/tjdevries/git/llvm-project")
endif()

message(STATUS "LLVM Repo: ${LLVM_REPO_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories("${LLVM_REPO_DIR}")
include_directories("${LLVM_REPO_DIR}/clang-tools-extra/clangd/")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (PATH_TO_LLVM)
  set (search_paths
    ${PATH_TO_LLVM}
    ${PATH_TO_LLVM}/lib/cmake
    ${PATH_TO_LLVM}/lib/cmake/llvm
    ${PATH_TO_LLVM}/lib/cmake/clang
    ${PATH_TO_LLVM}/share/clang/cmake/
    ${PATH_TO_LLVM}/share/llvm/cmake/
  )
endif()

message(STATUS "${search_paths}")

find_package(Clang REQUIRED CONFIG
             PATHS ${search_paths})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using ClangConfig.cmake in: ${CLANG_CMAKE_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
add_executable(lsif-clang
        src/LsifMain.cpp
        src/LsifSerialization.cpp
        src/LsifMerge.cpp
        src/LsifIndexAction.cpp
        ${BACKWARD_ENABLE})

# LLVM Dependencies
# target_include_directories(lsif-clang PUBLIC "/home/tjdevries/.cache/lsif-clang/llvm-project/")
# target_include_directories(lsif-clang PUBLIC "/home/tjdevries/.cache/lsif-clang/llvm-project/clang-tools-extra/clangd/")
target_link_libraries(lsif-clang stdc++)
target_link_libraries(lsif-clang clangDaemon)

# Backwards CPP Installation.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendored/backward-cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendored/backward-cpp)

add_backward(lsif-clang)
