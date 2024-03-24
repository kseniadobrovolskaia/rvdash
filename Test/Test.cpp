#include "RunTests.hpp"
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
  RunTests(); // Run rvdash and fill files in "Results" directory.
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

#define ADD_TEST(Num)                                                          \
  TEST(Test_rvdash, Test##Num) {                                               \
    EXPECT_TRUE(IsEqual(NameResults(Num), NameAnswers(Num)));                  \
  }

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
