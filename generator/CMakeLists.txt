cmake_minimum_required(VERSION 3.10)
project(SDL-Generator
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LLVM_PATH "Path to llvm/clang installation (where to find libclang)" ON)

find_package(LLVM REQUIRED CONFIG PATHS ${LLVM_PATH} NO_DEFAULT_PATH)

add_executable(sdl-generator main.cpp)