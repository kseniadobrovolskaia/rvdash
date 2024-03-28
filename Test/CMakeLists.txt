cmake_minimum_required(VERSION 3.14)
project(tests)

execute_process(COMMAND chmod +x ${CMAKE_SOURCE_DIR}/Test/TestGen.bash)
execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Test/TestGen.bash ${CMAKE_SOURCE_DIR}/Test)


# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_compile_options(-Wall -std=c++2a -fexceptions)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(
  rvdashTests
  
  RunTests.cpp
  Test.cpp
)
target_link_libraries(
  rvdashTests
  rvdash
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(rvdashTests)

