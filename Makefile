# Target application.
TARGET := app

# Static library.
LIB := libctest.a

# C compiler.
CC := gcc

# C compiler flags.
CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c11 -Iinclude

# Library source files.
LIB_SRC := source/ctest/impl/case.c \
           source/ctest/impl/helpers.c \
           source/ctest/impl/registry.c \
           source/ctest/ctest.c

# Library object files.
LIB_OBJ := $(LIB_SRC:.c=.o)

# Library header files.
LIB_HDR := $(wildcard include/ctest/*.h include/ctest/impl/*.h)

# Application source files.
APP_SRC := source/main.c

# Self-test programs.
TEST_BINS := tests/failures tests/duplicate tests/empty

# Build and run the application by default.
default: build run

# Build the static library.
lib: $(LIB)

$(LIB): $(LIB_OBJ)
	@ar rcs $(LIB) $(LIB_OBJ)

%.o: %.c $(LIB_HDR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Build the application.
build: $(LIB)
	@$(CC) $(APP_SRC) -o $(TARGET) $(CFLAGS) -L. -lctest

# Run the application.
run:
	@./$(TARGET)

# Build the self-test programs.
test-bins: $(TEST_BINS)

tests/failures: tests/failures.c $(LIB)
	@$(CC) $< -o $@ $(CFLAGS) -L. -lctest

tests/duplicate: tests/duplicate_a.c tests/duplicate_b.c $(LIB)
	@$(CC) tests/duplicate_a.c tests/duplicate_b.c -o $@ $(CFLAGS) -L. -lctest

tests/empty: tests/empty.c $(LIB)
	@$(CC) $< -o $@ $(CFLAGS) -L. -lctest

# Run the example and the self-tests, comparing their output with tests/expected/.
test:
	@ci/test.sh

# Clean the application.
clean:
	@rm -f $(TARGET) $(LIB) $(LIB_OBJ) $(TEST_BINS)

# Format all sources in place.
format:
	@ci/format.sh

# Check that all sources are formatted, without modifying them.
format-check:
	@ci/format.sh --check

.PHONY: default lib build run test-bins test clean format format-check
