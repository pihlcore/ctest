/**
 * @file Assertion helpers.
 */
#ifndef CTEST_IMPL_HELPERS_H
#define CTEST_IMPL_HELPERS_H

#include <stdbool.h>

#include "ctest/impl/case.h"

#define CTEST_IMPL_VALUE_BUF 64U // Buffer size for one formatted value.

/**
 * @brief Format a value into buf (of size CTEST_IMPL_VALUE_BUF), picking a formatter by type.
 *
 * @note Character types are printed as integers, matching their use as small numbers.
 *       Floating-point values are printed with enough digits to show their exact value.
 */
#define CTEST_IMPL_FORMAT_(buf, value)           \
    _Generic((value),                            \
        bool: ctest_impl_fmt_bool,               \
        char: ctest_impl_fmt_int,                \
        signed char: ctest_impl_fmt_int,         \
        unsigned char: ctest_impl_fmt_uint,      \
        short: ctest_impl_fmt_int,               \
        unsigned short: ctest_impl_fmt_uint,     \
        int: ctest_impl_fmt_int,                 \
        unsigned int: ctest_impl_fmt_uint,       \
        long: ctest_impl_fmt_int,                \
        unsigned long: ctest_impl_fmt_uint,      \
        long long: ctest_impl_fmt_int,           \
        unsigned long long: ctest_impl_fmt_uint, \
        float: ctest_impl_fmt_float,             \
        double: ctest_impl_fmt_double,           \
        long double: ctest_impl_fmt_ldouble,     \
        char*: ctest_impl_fmt_str,               \
        const char*: ctest_impl_fmt_str,         \
        default: ctest_impl_fmt_ptr)((buf), (value))

/** Shared implementation of EXPECT_EQ and EXPECT_NE; each operand is evaluated once. */
#define CTEST_IMPL_COMPARE_(a, b, op, negated, assertion)                                    \
    do                                                                                       \
    {                                                                                        \
        __auto_type ctest_a_ = (a);                                                          \
        __auto_type ctest_b_ = (b);                                                          \
        if (!(ctest_a_ op ctest_b_))                                                         \
        {                                                                                    \
            char ctest_sa_[CTEST_IMPL_VALUE_BUF];                                            \
            char ctest_sb_[CTEST_IMPL_VALUE_BUF];                                            \
            CTEST_IMPL_FORMAT_(ctest_sa_, ctest_a_);                                         \
            CTEST_IMPL_FORMAT_(ctest_sb_, ctest_b_);                                         \
            ctest_impl_fail(__FILE__, __LINE__, "%s failed: %s %s %s", assertion, ctest_sa_, \
                            negated, ctest_sb_);                                             \
        }                                                                                    \
    } while (false)

void ctest_impl_fmt_bool(char* buf, bool value);
void ctest_impl_fmt_int(char* buf, long long value);
void ctest_impl_fmt_uint(char* buf, unsigned long long value);
void ctest_impl_fmt_float(char* buf, float value);
void ctest_impl_fmt_double(char* buf, double value);
void ctest_impl_fmt_ldouble(char* buf, long double value);
void ctest_impl_fmt_str(char* buf, const char* value);
void ctest_impl_fmt_ptr(char* buf, const void* value);

#endif // CTEST_IMPL_HELPERS_H
