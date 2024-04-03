cmake_minimum_required(VERSION 3.14)

project(rvdash)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_SNIPPY_MODEL "Build libSnippyRVdash.so library for llvm-snippy" ON)


add_compile_options(-std=c++20)

set(SOURCE_EXE main/main.cpp
               main/Error.cpp
   )

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(rvdashSim ${SOURCE_EXE})

add_subdirectory(lib/Memory)
add_subdirectory(lib/rvdash)

if (BUILD_TESTS)
  add_subdirectory(Test)
endif (BUILD_TESTS)

if (BUILD_SNIPPY_MODEL)
  add_subdirectory(lib/SnippyRVdash)
endif (BUILD_SNIPPY_MODEL)

target_link_libraries(rvdashSim rvdash)
