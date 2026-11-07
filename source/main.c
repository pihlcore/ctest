/**
 * @file ctest usage example.
 */
#include <stdlib.h>

#include "ctest/ctest.h"

/**
 * @brief Test integer addition.
 *
 *        Add two integers and expect the result to be exact.
 */
TEST(Math, Addition) { EXPECT_EQ(2U + 3U, 5U); }

/**
 * @brief Test floating-point division.
 *
 *        Divide 1 by 3 and expect the result to be within tolerance.
 */
TEST(Math, FloatingPoint) { EXPECT_NEAR(1.0 / 3.0, 0.333, 1e-3); }

/**
 * @brief Test integer inequality.
 *
 *        Compare two different integers and expect them to be unequal.
 */
TEST(Math, Inequality) { EXPECT_NE(2U + 3U, 6U); }

/**
 * @brief Test boolean expressions.
 *
 *        Evaluate two boolean expressions and expect true and false respectively.
 */
TEST(Logic, BooleanChecks)
{
    EXPECT_TRUE(10U > 5U);
    EXPECT_FALSE(10U < 5U);
}

/**
 * @brief Run all test cases.
 *
 * @return 0 on success, 1 on failure.
 */
int main(void) { return ctest_run_all() ? EXIT_SUCCESS : EXIT_FAILURE; }
