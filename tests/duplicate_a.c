/**
 * @file Self-test: a test case name defined in two files (first definition).
 */
#include "ctest/ctest.h"

/**
 * @brief Test that a test case name defined in two files is detected (first definition).
 *
 *        Expect a duplicate warning, and only the first definition to run.
 */
TEST(Dup, Same) { EXPECT_TRUE(true); }
