/**
 * @file Test case implementation details.
 */
// Expose POSIX isatty() and fileno() in strict ISO C mode; must precede all includes.
#define _POSIX_C_SOURCE 200809L

#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ctest/impl/case.h"

#define MSG_SIZE 512U // Size of the failure message buffer, including the terminator.
#define EMPTY_LEN 0U  // Length of an empty string.
#define MIN_LEN 0     // Smallest valid vsnprintf() result; negative values signal an error.

#define START_IDX 0U // Index of the first character in a string.
#define SUCCESS 0    // Return value of C library functions such as strcmp() on success/match.

#define JMP_BUF_INIT 0 // Zero-initializer for the opaque jump buffer.
#define JMP_DIRECT 0   // Value setjmp() returns when called directly, not via longjmp().
#define JMP_FAILURE 1  // Value passed to longjmp() when an assertion fails.

#define STR_END '\0' // Null terminator ending a string.
#define NO_STR ""    // Empty string printed in place of a color code.
#define IS_TTY 1     // Value isatty() returns for a terminal.

#define GREEN "\033[32m"   // ANSI escape code for green text.
#define RED "\033[31m"     // ANSI escape code for red text.
#define NO_COLOR "\033[0m" // ANSI escape code resetting the text color.

/** Point to return to when an assertion fails in the running test case. */
static jmp_buf failure_jump = {JMP_BUF_INIT};

/** True while a test case is running, i.e. while failure_jump is valid. */
static volatile bool test_running = false;

/** Message describing the most recent assertion failure. */
static char failure_msg[MSG_SIZE] = {STR_END};

// -----------------------------------------------------------------------------
static inline bool match(const char* str1, const char* str2)
{
    return SUCCESS == strcmp(str1, str2);
}

// -----------------------------------------------------------------------------
static inline bool str_valid(const char* name)
{
    return (NULL != name) && (STR_END != name[START_IDX]);
}

// -----------------------------------------------------------------------------
static inline bool func_valid(const ctest_impl_case* test_case) { return NULL != test_case->func; }

// -----------------------------------------------------------------------------
/** Return the color code if stdout is a terminal, otherwise an empty string. */
static const char* color(const char* code)
{
    return (IS_TTY == isatty(fileno(stdout))) ? code : NO_STR;
}

// -----------------------------------------------------------------------------
bool ctest_impl_case_equal(const ctest_impl_case* a, const ctest_impl_case* b)
{
    return match(a->suite, b->suite) && match(a->name, b->name);
}

// -----------------------------------------------------------------------------
bool ctest_impl_case_is_valid(const ctest_impl_case* test_case)
{
    return str_valid(test_case->suite) && str_valid(test_case->name) && func_valid(test_case);
}

// -----------------------------------------------------------------------------
bool ctest_impl_case_run(const ctest_impl_case* test_case)
{
    if (!ctest_impl_case_is_valid(test_case)) { return false; }
    test_running = true;

    // Run the test; a failed assertion jumps back to setjmp() with JMP_FAILURE.
    if (JMP_DIRECT == setjmp(failure_jump))
    {
        test_case->func();
        test_running = false;
        printf("%sTest case %s.%s succeeded!%s\n", color(GREEN), test_case->suite, test_case->name,
               color(NO_COLOR));
        return true;
    }

    // An assertion failed: print the failure message in red.
    test_running = false;
    printf("%sTest case %s.%s failed: %s%s\n", color(RED), test_case->suite, test_case->name,
           failure_msg, color(NO_COLOR));
    return false;
}

// -----------------------------------------------------------------------------
void ctest_impl_fail(const char* file, int line, const char* fmt, ...)
{
    // Format the failure message; treat a formatting error as an empty message.
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(failure_msg, MSG_SIZE, fmt, args);
    va_end(args);
    const size_t msg_len = (MIN_LEN < len) ? (size_t)(len) : EMPTY_LEN;

    // Append the source location if the message wasn't truncated.
    if (MSG_SIZE > msg_len)
    {
        const size_t max_len = MSG_SIZE - msg_len;
        snprintf(failure_msg + msg_len, max_len, " (%s:%d)", file, line);
    }

    // Abort the running test case, or the whole program if no test is running.
    if (test_running) { longjmp(failure_jump, JMP_FAILURE); }
    fprintf(stderr, "%s\n", failure_msg);
    abort();
}
