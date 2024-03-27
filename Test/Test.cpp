#include "RunTests.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

const std::string getNameResults(unsigned NumTest, const std::string Dir) {
  const auto ResultDir = Dir + "/Results/";
  const auto File = "_TestResults.txt";
  return ResultDir + std::to_string(NumTest) + File;
}

const std::string getNameAnswers(unsigned NumTest, const std::string Dir) {
  const auto AnswerDir = Dir + "/Answers/";
  const auto File = "_TestAnswer.txt";
  return AnswerDir + std::to_string(NumTest) + File;
}

const std::string getNameData(unsigned NumTest, const std::string Dir) {
  const auto DataDir = Dir + "/Data/";
  const auto File = "_TestData.bin";
  return DataDir + std::to_string(NumTest) + File;
}

const std::string getNameAsm(unsigned NumTest, const std::string Dir) {
  const auto DataDir = Dir + "/Data/";
  const auto File = "_TestData.S";
  return DataDir + std::to_string(NumTest) + File;
}

/**
 * @brief IsEqual - This function, using a FileCheck,
 *                  finds the necessary execution elements (writes to
 *                  registers or memory) in the trace.
 */
::testing::AssertionResult IsEqual(unsigned NumTest,
                                   const std::string CurrTestDir) {
  auto NameAnswer = getNameAnswers(NumTest, CurrTestDir);
  auto NameResult = getNameResults(NumTest, CurrTestDir);

  std::string CheckCmd = "cat " + NameResult + " | FileCheck " + NameAnswer;
  auto Result = system(CheckCmd.c_str());
  auto BinRmCmd = "rm " + getNameData(NumTest, CurrTestDir);
  system(BinRmCmd.c_str());
  if (Result != 0) {
    return ::testing::AssertionFailure()
           << "The required trace elements were not found";
  } else {
    return ::testing::AssertionSuccess();
  }
}

/**
 * @brief TEST(CompileTests, Test0) - This function is not a test.
 *                                    It compiles all assembly tests into
 *                                    binary files ready for execution on
 *                                    the model.
 */
TEST(CompileTests, Test0) {
  const std::string NameResult = "Comp.log";
  std::ofstream ResultFile(NameResult);
  if (!ResultFile.is_open())
    rvdash::failWithError("Can't open file " + NameResult);
  compileAllTests(ResultFile);
  // This file is needed to print error messages into it during
  // test compilation. If any of the tests compiled with assembler,
  // linker or objcopy errors, then this file will contain a record
  // of these errors. Thus, if the file is empty (that is, Position == 0),
  // then compilation of all files was successful.
  auto Position = ResultFile.tellp();
  ResultFile.close();
  system(std::string("cat " + NameResult).c_str());
  system(std::string("rm " + NameResult).c_str());
  EXPECT_TRUE(Position == 0);
}

/**
 * @brief TEST - Tests.
 *               Expects to find the necessary elements in the trace.
 *
 */
#define ADD_TEST(Num, TestsName)                                               \
  TEST(TestsName, Test##Num) {                                                 \
    auto CurrTestDir = TestsName##TestsDir;                                    \
    auto NameData = getNameData(Num, CurrTestDir);                             \
    auto NameResult = getNameResults(Num, CurrTestDir);                        \
    runOneTest(NameData, NameResult);                                          \
    EXPECT_TRUE(IsEqual(Num, CurrTestDir));                                    \
  }

//-------------------------------------RVDASH_TESTS--------------------------------------
#include "rvdashTests/GenTests.h"
//------------------------------RVDASH_ERROR_HANDLING_TESTS------------------------------
#include "ErrorHandlingTests/GenTests.h"

#undef ADD_TEST
