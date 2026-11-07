/**
 * @file Test registry.
 */
#ifndef CTEST_IMPL_REGISTRY_H
#define CTEST_IMPL_REGISTRY_H

#include <stdbool.h>
#include <stddef.h>

#include "ctest/impl/case.h"

/**
 * @brief Add new test case.
 *
 * @param[in] test_case Test case to add.
 *
 * @note A warning is printed to stderr for each test case that is not added.
 *
 * @return True if the test case was added, false otherwise.
 */
bool ctest_impl_registry_add(ctest_impl_case test_case);

/**
 * @brief Get registered test cases.
 *
 * @param[out] count Number of registered test cases.
 *
 * @return The registered test cases, or NULL if there are none.
 */
const ctest_impl_case* ctest_impl_registry_cases(size_t* count);

/**
 * @brief Get the number of test cases that could not be registered.
 *
 * @return Number of invalid, duplicate or unallocated test cases.
 */
size_t ctest_impl_registry_rejected(void);

#endif // CTEST_IMPL_REGISTRY_H
