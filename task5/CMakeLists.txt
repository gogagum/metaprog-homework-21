cmake_minimum_required(VERSION 3.21)
project(task5)

set(CMAKE_CXX_COMPILER /usr/bin/clang++-13)
set(CMAKE_CXX_STANDARD 20)

add_executable(task5 main.cpp reflect.hpp)
