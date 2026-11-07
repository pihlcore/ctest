/**
 * @file Self-test: a test case name defined in two files (second definition).
 */
#include <stdlib.h>

#include "ctest/ctest.h"

/**
 * @brief Test that a test case name defined in two files is detected (second definition).
 *
 *        Expect a duplicate warning, and only the first definition to run.
 */
TEST(Dup, Same) { EXPECT_TRUE(true); }

/**
 * @brief Run all test cases.
 *
 * @return 0 on success, 1 on failure.
 */
int main(void) { return ctest_run_all() ? EXIT_SUCCESS : EXIT_FAILURE; }
