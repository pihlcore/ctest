/**
 * @file Self-test: every assertion macro, passing and failing, with all value types.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ctest/ctest.h"

#define NO_CALLS 0U // Number of next_call() calls before any test runs.
#define ONE_CALL 1U // Number of next_call() calls after a single evaluation.

#define VALUE 1       // Arbitrary signed integer.
#define OTHER_VALUE 2 // Signed integer greater than VALUE.
#define UVALUE 3U     // Arbitrary unsigned integer.

#define INT_FAR_A 10 // Integer far from INT_FAR_B.
#define INT_FAR_B 3  // Integer far from INT_FAR_A.
#define INT_TOL 2    // Integer tolerance smaller than |INT_FAR_A - INT_FAR_B|.

#define NEAR_A 1.0     // Floating-point value within DOUBLE_TOL of NEAR_B.
#define NEAR_B 1.05    // Floating-point value within DOUBLE_TOL of NEAR_A.
#define FAR_B 1.5      // Floating-point value farther than DOUBLE_TOL from NEAR_A.
#define DOUBLE_TOL 0.1 // Floating-point tolerance.

#define BYTE_VALUE 65U       // Arbitrary 8-bit value.
#define OTHER_BYTE_VALUE 66U // 8-bit value different from BYTE_VALUE.

#define FLOAT_VALUE 0.1F       // Arbitrary float.
#define OTHER_FLOAT_VALUE 0.2F // Float different from FLOAT_VALUE.

#define TENTH 0.1        // 0.1 as a double, which isn't exactly representable.
#define TWO_TENTHS 0.2   // 0.2 as a double, which isn't exactly representable.
#define THREE_TENTHS 0.3 // 0.3 as a double; not equal to TENTH + TWO_TENTHS.

#define LDOUBLE_VALUE 0.3L       // Arbitrary long double.
#define OTHER_LDOUBLE_VALUE 0.5L // Long double different from LDOUBLE_VALUE.

/** Number of calls to next_call(). */
static unsigned calls = NO_CALLS;

/** Count and return the number of calls; used to verify single evaluation. */
static unsigned next_call(void) { return ++calls; }

/**
 * @brief Test that all assertion macros pass on valid input.
 *
 *        Expect each assertion to hold, so the test case succeeds.
 */
TEST(Pass, AllMacros)
{
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
    EXPECT_EQ(VALUE, VALUE);
    EXPECT_NE(VALUE, OTHER_VALUE);
    EXPECT_NEAR(NEAR_A, NEAR_B, DOUBLE_TOL);
}

/**
 * @brief Test that assertion operands are evaluated only once.
 *
 *        Expect next_call() to have been called exactly once after being used in EXPECT_EQ.
 */
TEST(Pass, EvaluatesOnce)
{
    EXPECT_EQ(next_call(), ONE_CALL);
    EXPECT_EQ(calls, ONE_CALL);
}

/**
 * @brief Test that EXPECT_TRUE fails on a false expression.
 *
 *        Expect the failure message to show "false != true".
 */
TEST(Fail, True) { EXPECT_TRUE(VALUE > OTHER_VALUE); }

/**
 * @brief Test that EXPECT_FALSE fails on a true expression.
 *
 *        Expect the failure message to show "true != false".
 */
TEST(Fail, False) { EXPECT_FALSE(OTHER_VALUE > VALUE); }

/**
 * @brief Test that EXPECT_EQ fails on different signed integers.
 *
 *        Expect the failure message to show both integer values.
 */
TEST(Fail, EqInt) { EXPECT_EQ(VALUE, OTHER_VALUE); }

/**
 * @brief Test that EXPECT_NE fails on equal unsigned integers.
 *
 *        Expect the failure message to show both unsigned values.
 */
TEST(Fail, NeUnsigned) { EXPECT_NE(UVALUE, UVALUE); }

/**
 * @brief Test that EXPECT_NEAR fails on floating-point values outside the tolerance.
 *
 *        Expect the failure message to show the values, their difference and the tolerance.
 */
TEST(Fail, NearDouble) { EXPECT_NEAR(NEAR_A, FAR_B, DOUBLE_TOL); }

/**
 * @brief Test that EXPECT_NEAR fails on integers outside the tolerance.
 *
 *        Expect the failure message to show the values, their difference and the tolerance.
 */
TEST(Fail, NearInt) { EXPECT_NEAR(INT_FAR_A, INT_FAR_B, INT_TOL); }

/**
 * @brief Test that EXPECT_EQ prints 8-bit values as numbers.
 *
 *        Expect the failure message to show numbers rather than characters.
 */
TEST(Fail, Uint8)
{
    const uint8_t value = BYTE_VALUE;
    EXPECT_EQ(value, OTHER_BYTE_VALUE);
}

/**
 * @brief Test that EXPECT_EQ prints bool values as true/false.
 *
 *        Expect the failure message to show "false" for the bool operand.
 */
TEST(Fail, Bool)
{
    const bool value = false;
    EXPECT_EQ(value, true);
}

/**
 * @brief Test that EXPECT_EQ prints strings in quotes.
 *
 *        Expect the failure message to show both strings in quotes.
 */
TEST(Fail, String)
{
    const char* hi = "hi";
    const char* yo = "yo";
    EXPECT_EQ(hi, yo);
}

/**
 * @brief Test that EXPECT_EQ prints float values.
 *
 *        Expect the failure message to show both float values with minimal digits.
 */
TEST(Fail, Float)
{
    const float value = FLOAT_VALUE;
    EXPECT_EQ(value, OTHER_FLOAT_VALUE);
}

/**
 * @brief Test that EXPECT_EQ prints doubles with enough digits to show the difference.
 *
 *        Expect 0.1 + 0.2 to be shown as 0.30000000000000004, not 0.3.
 */
TEST(Fail, Double) { EXPECT_EQ(TENTH + TWO_TENTHS, THREE_TENTHS); }

/**
 * @brief Test that EXPECT_EQ prints long double values.
 *
 *        Expect the failure message to show both long double values with minimal digits.
 */
TEST(Fail, LongDouble)
{
    const long double value = LDOUBLE_VALUE;
    EXPECT_EQ(value, OTHER_LDOUBLE_VALUE);
}

/**
 * @brief Test that a failed assertion stops the test case.
 *
 *        Expect only the first failure to be reported.
 */
TEST(Fail, StopsAtFirstFailure)
{
    EXPECT_EQ(VALUE, OTHER_VALUE);
    EXPECT_TRUE(false);
}

/**
 * @brief Run all test cases.
 *
 * @return 0 on success, 1 on failure.
 */
int main(void) { return ctest_run_all() ? EXIT_SUCCESS : EXIT_FAILURE; }
