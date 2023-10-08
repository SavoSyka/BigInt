cmake_minimum_required(VERSION 3.24)
project(BigInt)

set(CMAKE_CXX_STANDARD 17)

add_executable(BigInt tests.cpp catch_main.cpp)
add_compile_options(g++ -sanitize=address,undefined -std=c++17)