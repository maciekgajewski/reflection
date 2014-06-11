project(relfection)
cmake_minimum_required(VERSION 2.8)

include_directories(.)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

add_subdirectory(data_types)
add_subdirectory(usage_examples)

