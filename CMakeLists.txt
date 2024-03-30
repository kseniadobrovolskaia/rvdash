cmake_minimum_required(VERSION 3.14)

project(rvdash)

add_compile_options(-std=c++20 -ggdb)

set(SOURCE_EXE main/main.cpp
               main/Error.cpp
   )

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(rvdashSim ${SOURCE_EXE})

add_subdirectory(lib/Memory)
add_subdirectory(lib/rvdash)
add_subdirectory(Test)

target_link_libraries(rvdashSim rvdash)
