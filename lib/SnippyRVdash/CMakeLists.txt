cmake_minimum_required(VERSION 3.14)

project(SnippyRVdash)

add_compile_options(-std=c++20)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCE_LIB SnippyRVdash.cpp
               VTable.cpp
               Error.cpp
   )

add_library(SnippyRVdash SHARED ${SOURCE_LIB})

target_link_libraries(SnippyRVdash rvdash)

