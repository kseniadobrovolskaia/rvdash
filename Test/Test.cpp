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

static void compileTest(const std::string CurrTestDir, unsigned NumTest) {
  const std::string NameLog = "Compile.log";
  std::ofstream LogFile(NameLog);
  if (!LogFile.is_open())
    rvdash::failWithError("Can't open file " + NameLog);
  compileOneTest(CurrTestDir, NumTest, LogFile);
  // This file is needed to print error messages into it during
  // test compilation. If test compiled with assembler,
  // linker or objcopy errors, then this file will contain a record
  // of these errors. Thus, if the file is empty (that is, Position == 0),
  // then compilation was successful.
  auto Position = LogFile.tellp();
  LogFile.close();
  system(std::string("cat " + NameLog).c_str());
  system(std::string("rm " + NameLog).c_str());
  EXPECT_TRUE(Position == 0);
  EXPECT_TRUE(4);
}

/**
 * @brief TEST - Tests.
 *               Expects to find the necessary elements in the trace.
 *
 */
#define ADD_TEST(Num, TestsName)                                               \
  TEST(TestsName, Test##Num) {                                                 \
    auto CurrTestDir = TestsName##TestsDir;                                    \
    compileTest(CurrTestDir, Num);                                             \
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
