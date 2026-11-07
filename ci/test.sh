#!/usr/bin/env bash
#
# Build and run the example and the self-tests, comparing output with tests/expected/.
#
# Usage:
#   ci/test.sh
set -euo pipefail

# Root directory.
ROOT_DIR="$(dirname "${BASH_SOURCE[0]}")/.."

# Number of failed checks.
FAILURES=0

################################################################################
# Run a program and check its exit code and combined stdout/stderr output.
# Globals:
#   FAILURES
# Arguments:
#   $1 - Program to run.
#   $2 - Expected exit code.
#   $3 - File holding the expected output.
################################################################################
check() {
    local program="$1"
    local expected_code="$2"
    local expected_output="$3"
    local output
    local code=0

    output=$("$program" 2>&1) || code=$?
    if [[ "$code" != "$expected_code" ]]
    then
        echo "FAIL: $program exited with $code, expected $expected_code."
        ((++FAILURES))
    elif ! diff -u "$expected_output" <(printf '%s\n' "$output")
    then
        echo "FAIL: $program output differs from $expected_output."
        ((++FAILURES))
    else
        echo "PASS: $program"
    fi
}

# Navigate to the root directory and build everything.
cd "$ROOT_DIR"
make --no-print-directory --silent build test-bins

# Run the example and the self-tests.
check ./app 0 tests/expected/app.out
check tests/failures 1 tests/expected/failures.out
check tests/duplicate 1 tests/expected/duplicate.out
check tests/empty 1 tests/expected/empty.out

# Fail if any check failed.
if ((FAILURES > 0))
then
    echo "$FAILURES check(s) failed."
    exit 1
fi
echo "All checks passed."
