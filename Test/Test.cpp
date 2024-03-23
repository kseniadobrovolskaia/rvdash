#include "Run_tests.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

::testing::AssertionResult IsEqual(const std::string &NameResults,
                                   const std::string &NameAnswer) {
  std::string Command =
      "diff " + NameResults + " " + NameAnswer + " > Diff.txt";
  system(Command.c_str());
  std::ifstream IsEqual;
  IsEqual.open("Diff.txt");

  if (!(IsEqual.is_open())) {
    std::cerr << "File \"is_equal.txt\" did not open\n";
    exit(EXIT_FAILURE);
  }

  Command.clear();
  IsEqual >> Command;

  if (Command.size() != 0) {
    return ::testing::AssertionFailure() << "Files are not equal";
  } else {
    return ::testing::AssertionSuccess();
  }
}

//--------------------------------------RUN_rvdash---------------------------------------

TEST(RunTests, Test0) {
  Run_tests(); // Run rvdash and fill files in "Results" directory.
}

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

TEST(Test_rvdash, Test1) {
  EXPECT_TRUE(IsEqual(NameResults(1), NameAnswers(1)));
}
