cmake_minimum_required(VERSION 3.20)
project(task2)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)

add_executable(task2 main.cpp type_lists.hpp type_tuples.hpp type_lists_concepts.hpp impl/get_from_ttuple.hpp impl/get_from_typelist.hpp impl/error_return.hpp impl/convert_to_type_tuple.hpp impl/skip_while.hpp impl/filter.hpp impl/logic_not.hpp impl/drop.hpp impl/foldl.hpp)
