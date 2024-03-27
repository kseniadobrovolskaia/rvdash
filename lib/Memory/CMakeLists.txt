cmake_minimum_required(VERSION 3.14)

project(Memory)

add_compile_options(-Wall -std=c++20 -fexceptions)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCE_LIB Memory.cpp
   )


add_library(Memory STATIC ${SOURCE_LIB})

