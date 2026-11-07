/**
 * @file Assertion helper implementation details.
 */
#include <stdio.h>
#include <stdlib.h>

#include "ctest/impl/helpers.h"

#define MIN_PRECISION 6  // Fewest significant digits used to print a floating-point value.
#define MAX_PRECISION 36 // Most significant digits needed to print any long double exactly.

/**
 * @brief Format a floating-point value with the shortest precision (at least MIN_PRECISION digits)
 * that parses back to the same value of its type, so e.g. 0.1 + 0.2 isn't shown as 0.3.
 */
#define FORMAT_FLOATING(buf, value, type, parse)                                           \
    do                                                                                     \
    {                                                                                      \
        for (int precision = MIN_PRECISION; precision <= MAX_PRECISION; ++precision)       \
        {                                                                                  \
            snprintf(buf, CTEST_IMPL_VALUE_BUF, "%.*Lg", precision, (long double)(value)); \
            if ((type)(parse(buf, NULL)) == (value)) { return; }                           \
        }                                                                                  \
    } while (false)

// -----------------------------------------------------------------------------
void ctest_impl_fmt_bool(char* buf, bool value)
{
    snprintf(buf, CTEST_IMPL_VALUE_BUF, "%s", value ? "true" : "false");
}

// -----------------------------------------------------------------------------
void ctest_impl_fmt_int(char* buf, long long value)
{
    snprintf(buf, CTEST_IMPL_VALUE_BUF, "%lld", value);
}

// -----------------------------------------------------------------------------
void ctest_impl_fmt_uint(char* buf, unsigned long long value)
{
    snprintf(buf, CTEST_IMPL_VALUE_BUF, "%llu", value);
}

// -----------------------------------------------------------------------------
void ctest_impl_fmt_float(char* buf, float value) { FORMAT_FLOATING(buf, value, float, strtof); }

// -----------------------------------------------------------------------------
void ctest_impl_fmt_double(char* buf, double value) { FORMAT_FLOATING(buf, value, double, strtod); }

// -----------------------------------------------------------------------------
void ctest_impl_fmt_ldouble(char* buf, long double value)
{
    FORMAT_FLOATING(buf, value, long double, strtold);
}

// -----------------------------------------------------------------------------
void ctest_impl_fmt_str(char* buf, const char* value)
{
    if (NULL == value) { snprintf(buf, CTEST_IMPL_VALUE_BUF, "(null)"); }
    else { snprintf(buf, CTEST_IMPL_VALUE_BUF, "\"%s\"", value); }
}

// -----------------------------------------------------------------------------
void ctest_impl_fmt_ptr(char* buf, const void* value)
{
    snprintf(buf, CTEST_IMPL_VALUE_BUF, "%p", value);
}
