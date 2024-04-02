cmake_minimum_required(VERSION 3.14)

project(rvdash)

add_compile_options(-std=c++20 -fPIC)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCE_LIB InstructionSet/InstructionSet.cpp
               InstructionSet/Instruction.cpp
               InstructionSet/RV32I/InstructionSet.cpp
 )

add_library(rvdash STATIC ${SOURCE_LIB})

