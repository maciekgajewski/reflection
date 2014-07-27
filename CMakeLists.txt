project(relfection)
cmake_minimum_required(VERSION 2.8.8)

include_directories(.)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # usewd by clang-check

add_subdirectory(data_types)
add_subdirectory(usage_examples)
add_subdirectory(clang_generator)

