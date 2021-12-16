cmake_minimum_required(VERSION 3.13)
project(task4)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++-13")
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-O2)
add_executable(task4 main.cpp enum_traits.hpp)
