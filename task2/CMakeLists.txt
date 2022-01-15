cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
project(task2_gcc)
add_executable(task2_gcc main.cpp type_lists.hpp type_tuples.hpp src/type_lists_concepts.hpp src/impl/get_from_ttuple.hpp src/impl/get_from_typelist.hpp src/impl/error_return.hpp src/impl/convert_to_type_tuple.hpp src/impl/skip_while.hpp src/impl/filter.hpp src/impl/logic_not.hpp src/impl/drop.hpp src/impl/foldl.hpp src/inits.hpp src/take.hpp src/cycle.hpp src/impl/scanl.hpp src/impl/take_while.hpp src/zip2_to_lists.hpp src/cons.hpp src/impl/zip_to_list.hpp src/map.hpp src/impl/skip_every_nth.hpp)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(task2_clang)
add_executable(task2_clang main.cpp type_lists.hpp type_tuples.hpp src/type_lists_concepts.hpp src/impl/get_from_ttuple.hpp src/impl/get_from_typelist.hpp src/impl/error_return.hpp src/impl/convert_to_type_tuple.hpp src/impl/skip_while.hpp src/impl/filter.hpp src/impl/logic_not.hpp src/impl/drop.hpp src/impl/foldl.hpp src/inits.hpp src/take.hpp src/cycle.hpp src/impl/scanl.hpp src/impl/take_while.hpp src/zip2_to_lists.hpp src/cons.hpp src/impl/zip_to_list.hpp src/map.hpp src/impl/skip_every_nth.hpp)

set(CMAKE_CXX_COMPILER "/home/gogagum/intel/oneapi/compiler/latest/linux/bin/icx")  #TODO
project(task2_icx)
add_executable(task2_icx main.cpp type_lists.hpp type_tuples.hpp src/type_lists_concepts.hpp src/impl/get_from_ttuple.hpp src/impl/get_from_typelist.hpp src/impl/error_return.hpp src/impl/convert_to_type_tuple.hpp src/impl/skip_while.hpp src/impl/filter.hpp src/impl/logic_not.hpp src/impl/drop.hpp src/impl/foldl.hpp src/inits.hpp src/take.hpp src/cycle.hpp src/impl/scanl.hpp src/impl/take_while.hpp src/zip2_to_lists.hpp src/cons.hpp src/impl/zip_to_list.hpp src/map.hpp src/impl/skip_every_nth.hpp)
