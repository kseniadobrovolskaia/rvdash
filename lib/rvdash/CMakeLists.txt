cmake_minimum_required(VERSION 3.14)

project(rvdash)

add_compile_options(-Wall -std=c++20 -fexceptions)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCE_LIB InstructionSet.cpp
               Instruction.cpp
               RV32I/InstructionSet.cpp
   )


add_library(rvdash STATIC ${SOURCE_LIB})

