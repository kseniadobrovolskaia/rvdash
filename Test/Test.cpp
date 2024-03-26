#include "RunTests.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

static auto getNameResults(unsigned NumTest) {
  const char *Dir = "../../Test/rvdashTests/Results/";
  const char *File = "_TestResults.txt";
  return Dir + std::to_string(NumTest) + File;
}

static auto getNameAnswers(unsigned NumTest) {
  const char *Dir = "../../Test/rvdashTests/Answers/";
  const char *File = "_TestAnswer.txt";
  return Dir + std::to_string(NumTest) + File;
}

/**
 * @brief IsEqual - This function, using a FileCheck,
 *                  finds the necessary execution elements (writes to
 *                  registers or memory) in the trace.
 */
::testing::AssertionResult IsEqual(unsigned Name) {
  static unsigned NumTest = 0;
  NumTest++;
  auto NameAnswer = getNameAnswers(NumTest);
  auto NameResults = getNameResults(NumTest);

  std::string Command = "cat " + NameResults + " | FileCheck " + NameAnswer;
  auto Result = system(Command.c_str());
  auto RmAsm = "rm ../../Test/rvdashTests/Data/" + std::to_string(NumTest) +
               "_TestData.bin";
  system(RmAsm.c_str());
  if (Result != 0) {
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

#define ADD_TEST(Num)                                                          \
  TEST(Test_rvdash, Test##Num) { EXPECT_TRUE(IsEqual(Num)); }

/**
 * @brief TEST - Tests.
 *               Expects to find the necessary elements in the trace.
 *
 */

#include "GenTests.h"

#undef ADD_TEST
