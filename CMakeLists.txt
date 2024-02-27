cmake_minimum_required(VERSION 3.14)

project(rvdash)

add_compile_options(-Wall -std=c++20 -fexceptions)

set(SOURCE_EXE ./rvdash/main.cpp
               ./rvdash/Error.cpp)

include_directories(include)

add_executable(rvdash ${SOURCE_EXE})

#add_subdirectory(lib/Memory)
add_subdirectory(lib/InstructionSet)
#add_subdirectory(Tests)

target_link_libraries(rvdash InstructionSet)
