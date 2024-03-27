cmake_minimum_required(VERSION 3.14)

project(rvdash)

add_compile_options(-Wall -std=c++20 -pedantic -Wextra -fstack-usage -fexceptions -ggdb)

set(SOURCE_EXE main/main.cpp
               main/Error.cpp)

include_directories(include)

add_executable(rvdashSim ${SOURCE_EXE})

add_subdirectory(lib/Memory)
add_subdirectory(lib/rvdash)
add_subdirectory(Test)

target_link_libraries(rvdashSim rvdash)
