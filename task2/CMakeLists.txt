cmake_minimum_required(VERSION 3.20)
project(task2)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)

add_executable(task2 main.cpp type_lists.hpp type_tuples.hpp src/type_lists_concepts.hpp src/impl/get_from_ttuple.hpp src/impl/get_from_typelist.hpp src/impl/error_return.hpp src/impl/convert_to_type_tuple.hpp src/impl/skip_while.hpp src/impl/filter.hpp src/impl/logic_not.hpp src/impl/drop.hpp src/impl/foldl.hpp src/inits.hpp src/take.hpp src/cycle.hpp src/impl/scanl.hpp src/impl/take_while.hpp)
