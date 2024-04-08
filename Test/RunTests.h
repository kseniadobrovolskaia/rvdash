#ifndef RUN_TESTS_H
#define RUN_TESTS_H

#include <string>

void compileOneTest(const std::string CurrTestDir, unsigned NumTest,
                    std::ostream &LogFile);
void runOneTest(const std::string NameData, std::ostream &ResultFile);
void runOneSnippyModelTest(const std::string NameYaml,
                           const std::string &NameResult);

namespace rvdash {
void failWithError(const std::string &Msg);
}

const std::string getNameAsm(unsigned NumTest, const std::string Dir);
const std::string getNameData(unsigned NumTest, const std::string Dir);

const std::string TestDir = "../../Test";
const std::string rvdashTestsDir = TestDir + "/rvdashTests";
const std::string ErrorHandlingTestsDir = TestDir + "/ErrorHandlingTests";
const std::string SnippyRVdashTestsDir = TestDir + "/SnippyRVdashTests";
const std::string SnippyPath = TestDir + "/Snippy/snippy-1.0/llvm-snippy";

#endif // RUN_TESTS_H
