/**
 * @file Test registry implementation details.
 */
#include <stdio.h>
#include <stdlib.h>

#include "ctest/impl/case.h"
#include "ctest/impl/registry.h"

#define START_IDX 0U         // Index of the first test case.
#define EMPTY 0U             // Size of an empty registry.
#define INITIAL_CAPACITY 16U // Number of test cases allocated on the first registration.
#define GROWTH_FACTOR 2U     // Factor by which the capacity grows when the registry is full.

/** Registered test cases. */
static ctest_impl_case* test_cases = NULL;

/** Number of registered test cases. */
static size_t test_count = EMPTY;

/** Number of test cases that fit in the allocated storage. */
static size_t test_capacity = EMPTY;

/** Number of test cases that could not be registered. */
static size_t rejected_count = EMPTY;

// -----------------------------------------------------------------------------
static bool case_exists(const ctest_impl_case* test_case)
{
    // Iterate through existing test cases, return true on match with the given test case.
    for (size_t i = START_IDX; i < test_count; ++i)
    {
        if (ctest_impl_case_equal(&test_cases[i], test_case)) { return true; }
    }
    return false;
}

// -----------------------------------------------------------------------------
/** Count a test case that could not be registered, then return false. */
static bool reject(void)
{
    ++rejected_count;
    return false;
}

// -----------------------------------------------------------------------------
bool ctest_impl_registry_add(ctest_impl_case test_case)
{
    // Check test case, warn and return false if invalid or if it already exists.
    if (!ctest_impl_case_is_valid(&test_case))
    {
        fprintf(stderr, "ctest warning: Ignoring invalid test case!\n");
        return reject();
    }
    if (case_exists(&test_case))
    {
        fprintf(stderr, "ctest warning: Test case %s.%s is defined more than once!\n",
                test_case.suite, test_case.name);
        return reject();
    }

    // Grow storage if needed.
    if (test_count == test_capacity)
    {
        const size_t capacity =
            (EMPTY != test_capacity) ? test_capacity * GROWTH_FACTOR : INITIAL_CAPACITY;
        ctest_impl_case* cases = realloc(test_cases, capacity * sizeof(*cases));
        if (NULL == cases)
        {
            fprintf(stderr, "ctest warning: Out of memory, test case %s.%s not registered!\n",
                    test_case.suite, test_case.name);
            return reject();
        }
        test_cases    = cases;
        test_capacity = capacity;
    }
    // Add new test case.
    test_cases[test_count++] = test_case;
    return true;
}

// -----------------------------------------------------------------------------
const ctest_impl_case* ctest_impl_registry_cases(size_t* count)
{
    *count = test_count;
    return test_cases;
}

// -----------------------------------------------------------------------------
size_t ctest_impl_registry_rejected(void) { return rejected_count; }
