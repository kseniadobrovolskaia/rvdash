#ifndef RUN_TESTS_H
#define RUN_TESTS_H

#include <string>

void compileOneTest(const std::string CurrTestDir, unsigned NumTest,
                    std::ostream &LogFile);
void runOneTest(const std::string NameData, const std::string NameResult);

namespace rvdash {
void failWithError(const std::string &Msg);
}

const std::string getNameAsm(unsigned NumTest, const std::string Dir);
const std::string getNameData(unsigned NumTest, const std::string Dir);
const std::string getNameAnswers(unsigned NumTest, const std::string Dir);
const std::string getNameResults(unsigned NumTest, const std::string Dir);

const std::string TestDir = "../../Test";
const std::string rvdashTestsDir = TestDir + "/rvdashTests";
const std::string ErrorHandlingTestsDir = TestDir + "/ErrorHandlingTests";

#endif // RUN_TESTS_H
