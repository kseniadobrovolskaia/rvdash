cmake_minimum_required(VERSION 3.14)

project(InstructionSet)

add_compile_options(-Wall -std=c++20 -fexceptions)

include_directories(../../include)

set(SOURCE_LIB InstructionSet.cpp
               Instruction.cpp
               ./RV32I/InstructionSet.cpp
               ./M/InstructionSet.cpp
   )


add_library(InstructionSet STATIC ${SOURCE_LIB})

