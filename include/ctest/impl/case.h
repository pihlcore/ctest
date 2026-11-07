/**
 * @file Test case. Implementation detail; not part of the public API.
 */
#ifndef CTEST_IMPL_CASE_H
#define CTEST_IMPL_CASE_H

#include <stdbool.h>

#define CTEST_IMPL_FMT_IDX 3U  // Position of the format string in ctest_impl_fail().
#define CTEST_IMPL_ARGS_IDX 4U // Position of the first format argument in ctest_impl_fail().

/** Test function. */
typedef void (*ctest_impl_func)(void);

/**
 * @brief Test case.
 */
typedef struct
{
    /** Test suite. */
    const char* suite;

    /** Test name. */
    const char* name;

    /** Test function. */
    ctest_impl_func func;
} ctest_impl_case;

/**
 * @brief Match test cases by suite and name.
 *
 * @param[in] a First test case.
 * @param[in] b Second test case.
 *
 * @return True on match, otherwise false.
 */
bool ctest_impl_case_equal(const ctest_impl_case* a, const ctest_impl_case* b);

/**
 * @brief Check if the test case is valid.
 *
 * @param[in] test_case Test case to check.
 *
 * @return True if valid, false otherwise.
 */
bool ctest_impl_case_is_valid(const ctest_impl_case* test_case);

/**
 * @brief Run test case and print the result.
 *
 * @param[in] test_case Test case to run.
 *
 * @return True on success, false on failure.
 */
bool ctest_impl_case_run(const ctest_impl_case* test_case);

/**
 * @brief Report an assertion failure and abort the running test case.
 *
 *        Never returns while a test is running. If called outside a test, the message is
 *        printed and the program aborts.
 *
 * @param[in] file Source file of the assertion.
 * @param[in] line Source line of the assertion.
 * @param[in] fmt  printf-style failure message, followed by its arguments.
 */
__attribute__((noreturn, format(printf, CTEST_IMPL_FMT_IDX, CTEST_IMPL_ARGS_IDX))) void
ctest_impl_fail(const char* file, int line, const char* fmt, ...);

#endif // CTEST_IMPL_CASE_H
