project(relfection)
cmake_minimum_required(VERSION 2.8.8)

include_directories(.)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # required by reflection generator (by clang tool fmwk actually)


# point cmake to own redistributable files, to use them in tests and examples
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/redist/cmake")

add_subdirectory(example_reflected_lib)
add_subdirectory(tests)

