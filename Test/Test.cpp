#include "RunTests.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

/**
 * @brief IsEqual - This function, using a FileCheck,
 *                  finds the necessary execution elements (writes to
 *                  registers or memory) in the trace.
 */
::testing::AssertionResult IsEqual(const std::string &NameResults,
                                   const std::string &NameAnswer) {
  const char *DiffFile = "Diff.txt";
  std::string Command =
      "cat " + NameResults + " | FileCheck " + NameAnswer + " > " + DiffFile;

  system(Command.c_str());
  std::ifstream IsEqual;
  IsEqual.open(DiffFile);

  if (!(IsEqual.is_open())) {
    std::cerr << "File " << DiffFile << " did not open\n";
    exit(EXIT_FAILURE);
  }

  Command.clear();
  IsEqual >> Command;

  if (Command.size() != 0) {
    return ::testing::AssertionFailure()
           << "The required trace elements were not found";
  } else {
    return ::testing::AssertionSuccess();
  }
}

//--------------------------------------RUN_rvdash---------------------------------------

/**
 * @brief TEST(RunTests, Test0) - This function is not a test.
 *                                It starts the compilation and generation
 *                                of all traces (fill files in "Results"
 *                                directiry.
 */
TEST(RunTests, Test0) { RunTests(); }

//-------------------------------------RVDASH_TESTS--------------------------------------

static std::string NameResults(int NumTest) {
  std::string Str1 = "../../Test/rvdashTests/Results/";
  std::string Str2 = "_TestResults.txt";
  return Str1 + std::to_string(NumTest) + Str2;
}

static std::string NameAnswers(int NumTest) {
  std::string Str1 = "../../Test/rvdashTests/Answers/";
  std::string Str2 = "_TestAnswer.txt";
  return Str1 + std::to_string(NumTest) + Str2;
}

#define ADD_TEST(Num)                                                          \
  TEST(Test_rvdash, Test##Num) {                                               \
    EXPECT_TRUE(IsEqual(NameResults(Num), NameAnswers(Num)));                  \
  }

/**
 * @brief TEST - Tests.
 *               Expects to find the necessary elements in the trace.
 *
 */
ADD_TEST(1);
ADD_TEST(2);
ADD_TEST(3);
ADD_TEST(4);
ADD_TEST(5);
ADD_TEST(6);
ADD_TEST(7);
ADD_TEST(8);
ADD_TEST(9);
ADD_TEST(10);

#undef ADD_TEST
