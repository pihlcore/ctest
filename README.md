# ctest
A lightweight unit test framework for C implementations written at PihlCore. Tests are registered automatically via macros, and assertion failures report the expression, values, and source location. It is simple enough to also serve as a teaching tool for interns.

---

## Features
* Automatic test registration; no manual setup required.
* Colored output on a terminal: green on success, red on failure. Plain text when output is redirected, e.g. in CI logs.
* Assertion failures show actual values and source file/line.
* A failed assertion stops the current test case; remaining test cases still run.
* Each assertion operand is evaluated exactly once.
* Test cases that can't be registered, e.g. duplicate names, are reported and fail the run.
* Simple to integrate as a Git submodule.

## Requirements
* C11 or later (`-std=c11` works; `-Wpedantic` is clean), compiled with GCC or Clang.
  The framework uses the GCC/Clang extensions `__attribute__((constructor))` for automatic registration and `__auto_type` for single evaluation of operands.
* A POSIX system (e.g. Linux or macOS), for `isatty()` to detect whether output goes to a terminal.

---

## Macros

| Macro | Description |
|---|---|
| `TEST(suite, name)` | Define a test case belonging to a suite |
| `EXPECT_TRUE(expr)` | Fail if `expr` is not true |
| `EXPECT_FALSE(expr)` | Fail if `expr` is not false |
| `EXPECT_EQ(a, b)` | Fail if `a != b` |
| `EXPECT_NE(a, b)` | Fail if `a == b` |
| `EXPECT_NEAR(a, b, tol)` | Fail if `\|a - b\| > tol` (arithmetic values only) |

Notes:
* `EXPECT_EQ`/`EXPECT_NE` compare with `==`/`!=`, so strings (`char*`) are compared by pointer, not by content. Use `EXPECT_TRUE(0 == strcmp(a, b))` to compare contents.
* In C, `true` and `false` are the integers `1` and `0`, so `EXPECT_EQ(flag, true)` prints the expected value as `1`. Prefer `EXPECT_TRUE(flag)`.
* Each suite/name pair must be unique. Within a file, a duplicate is a compile error; across files, a warning is printed, only the first test case runs, and `ctest_run_all()` returns false.
* A failed assertion leaves the test case immediately, so cleanup later in the same test case (`free()`, `fclose()`, ...) is skipped. Release resources before the assertions, or accept the leak in test code.

---

## API
The only function you call directly is `ctest_run_all()`, declared in `ctest/ctest.h`:

```c
bool ctest_run_all(void); // True if all tests succeeded; false if any failed, none exist, or some couldn't be registered.
```

Everything under `ctest/impl/` and everything prefixed `ctest_impl_` or `CTEST_IMPL_` is internal and may change without notice.

---

## Writing tests
Include `ctest/ctest.h` and use the `TEST` macro to define test cases. Each test case belongs to a named suite:

```c
#include <stdlib.h>

#include "ctest/ctest.h"
#include "my_counter.h"

/**
 * @brief Test counter initialization.
 *
 *        Create a counter, expect it to start at zero and be valid.
 */
TEST(Counter, Init)
{
    my_counter counter;
    my_counter_init(&counter);
    EXPECT_EQ(my_counter_value(&counter), 0);
    EXPECT_TRUE(my_counter_is_valid(&counter));
}

/**
 * @brief Test floating-point addition.
 *
 *        Add two numbers and expect the result to be within tolerance.
 */
TEST(Math, Addition)
{
    EXPECT_NEAR(1.0 + 2.0, 3.0, 1e-9);
}

/**
 * @brief Run all test cases.
 *
 * @return EXIT_SUCCESS if all tests succeeded, otherwise EXIT_FAILURE.
 */
int main(void)
{
    return ctest_run_all() ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

Failure output:

```
Test case Counter.Init failed: EXPECT_EQ(my_counter_value(&counter), 0) failed: 3 != 0 (main.c:15)
```

---

## Integration as a Git submodule
Add `ctest` to your project:

```bash
git submodule add <url> libs/ctest
git submodule update --init --recursive
```

In your Makefile, build the library and link against it:

```makefile
CTEST_DIR := libs/ctest
CTEST_LIB := $(CTEST_DIR)/libctest.a

$(CTEST_LIB):
	$(MAKE) -C $(CTEST_DIR) lib

$(TARGET): $(CTEST_LIB) $(SRC)
	gcc $(SRC) -o $(TARGET) -std=c11 -I$(CTEST_DIR)/include -L$(CTEST_DIR) -lctest
```

---

## Building the library

```bash
make lib   # Builds libctest.a.
make       # Builds and runs the bundled example.
make test  # Runs the example and the self-tests in tests/, comparing output with tests/expected/.
make clean # Removes all build artifacts.
```

---

## Formatting
Sources are formatted with `clang-format` version 18 (see [`.clang-format`](./.clang-format)). Install it with `sudo apt -y install clang-format-18`.

```bash
make format       # Formats all sources in place.
make format-check # Checks formatting without modifying files; used in CI.
```

---
