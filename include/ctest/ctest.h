/**
 * @file ctest framework API.
 */
#ifndef CTEST_CTEST_H
#define CTEST_CTEST_H

#include <stdbool.h>

#include "ctest/impl/case.h"
#include "ctest/impl/helpers.h"
#include "ctest/impl/registry.h"

/**
 * @brief Define a test case.
 *
 *        The test is registered automatically before main() runs.
 *
 * @param[in] suite Test suite.
 * @param[in] name  Test name.
 */
#define TEST(suite, name)                                                                       \
    static void suite##_##name##_ctest_func(void);                                              \
    __attribute__((constructor)) static void suite##_##name##_ctest_register(void)              \
    {                                                                                           \
        ctest_impl_registry_add((ctest_impl_case){#suite, #name, suite##_##name##_ctest_func}); \
    }                                                                                           \
    static void suite##_##name##_ctest_func(void)

/**
 * @brief Expect expression to be true.
 *
 * @param[in] expr Expression to evaluate.
 */
#define EXPECT_TRUE(expr)                                                                        \
    do                                                                                           \
    {                                                                                            \
        if (!(expr))                                                                             \
        {                                                                                        \
            ctest_impl_fail(__FILE__, __LINE__, "EXPECT_TRUE(" #expr ") failed: false != true"); \
        }                                                                                        \
    } while (false)

/**
 * @brief Expect expression to be false.
 *
 * @param[in] expr Expression to evaluate.
 */
#define EXPECT_FALSE(expr)                                                                        \
    do                                                                                            \
    {                                                                                             \
        if (expr)                                                                                 \
        {                                                                                         \
            ctest_impl_fail(__FILE__, __LINE__, "EXPECT_FALSE(" #expr ") failed: true != false"); \
        }                                                                                         \
    } while (false)

/**
 * @brief Expect two values to be equal.
 *
 *        Values are compared with ==; strings are compared by pointer, not by content.
 *
 * @param[in] a First value.
 * @param[in] b Second value.
 */
#define EXPECT_EQ(a, b) CTEST_IMPL_COMPARE_(a, b, ==, "!=", "EXPECT_EQ(" #a ", " #b ")")

/**
 * @brief Expect two values to not be equal.
 *
 * @param[in] a First value.
 * @param[in] b Second value.
 */
#define EXPECT_NE(a, b) CTEST_IMPL_COMPARE_(a, b, !=, "==", "EXPECT_NE(" #a ", " #b ")")

/**
 * @brief Expect two arithmetic values to be within a given tolerance of each other.
 *
 * @param[in] a   First value.
 * @param[in] b   Second value.
 * @param[in] tol Tolerance.
 */
#define EXPECT_NEAR(a, b, tol)                                                                 \
    do                                                                                         \
    {                                                                                          \
        const long double ctest_a_   = (long double)(a);                                       \
        const long double ctest_b_   = (long double)(b);                                       \
        const long double ctest_tol_ = (long double)(tol);                                     \
        const long double ctest_d_ =                                                           \
            ctest_a_ > ctest_b_ ? ctest_a_ - ctest_b_ : ctest_b_ - ctest_a_;                   \
        if (!(ctest_d_ <= ctest_tol_))                                                         \
        {                                                                                      \
            ctest_impl_fail(__FILE__, __LINE__,                                                \
                            "EXPECT_NEAR(" #a ", " #b ", " #tol ") failed: |%Lg - %Lg| = %Lg " \
                            "> %Lg",                                                           \
                            ctest_a_, ctest_b_, ctest_d_, ctest_tol_);                         \
        }                                                                                      \
    } while (false)

/**
 * @brief Run all registered tests.
 *
 * @return True if all tests succeeded, false otherwise.
 */
bool ctest_run_all(void);

#endif // CTEST_CTEST_H
