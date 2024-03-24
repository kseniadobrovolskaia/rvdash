#ifndef RUN_TESTS_H
#define RUN_TESTS_H

/**
 * @brief RunTests - Starts the execution of rvdash programs,
 *                    the results are written to files in the "Results"
 *                    directory. Then runs gtest to compare the files
 *                    in the "Results" directory with the files in the
 *                    "Answers" directory (contains the correct
 *                    program output).
 */
void RunTests();

#endif // RUN_TESTS_H
