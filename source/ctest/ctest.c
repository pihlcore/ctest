/**
 * @file ctest implementation.
 */
#include <stddef.h>
#include <stdio.h>

#include "ctest/ctest.h"
#include "ctest/impl/case.h"
#include "ctest/impl/registry.h"

#define START_IDX 0U // Index of the first test case.
#define NONE 0U      // Zero test cases.

// -----------------------------------------------------------------------------
bool ctest_run_all(void)
{
    // Retrieve registered test cases, return false if missing.
    size_t count                 = NONE;
    const ctest_impl_case* cases = ctest_impl_registry_cases(&count);
    if (NONE == count)
    {
        printf("No test cases registered!\n");
        fflush(stdout);
        return false;
    }
    size_t failed = NONE;

    // Run each test case one by one, count failures.
    for (size_t i = START_IDX; i < count; ++i)
    {
        if (!ctest_impl_case_run(&cases[i])) { ++failed; }
    }
    // Print summary, then return true if all tests succeeded and none were rejected.
    printf("%zu out of %zu test cases succeeded!\n", count - failed, count);
    const size_t rejected = ctest_impl_registry_rejected();
    if (NONE != rejected)
    {
        printf("%zu test case(s) could not be registered, see warnings above!\n", rejected);
    }
    fflush(stdout);
    return (NONE == failed) && (NONE == rejected);
}
