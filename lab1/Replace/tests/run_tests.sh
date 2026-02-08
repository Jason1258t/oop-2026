#!/bin/bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_DIR="$(cd "$ROOT_DIR/.." && pwd)"
BIN="$REPO_DIR/Replace"

# Ensure binary exists
if [ ! -x "$BIN" ]; then
  echo "Binary $BIN not found or not executable. Compiling..."
  (cd "$REPO_DIR" && clang++ -std=c++17 -g Replace.cpp -o Replace)
fi

echo "Running -h test..."
HELP_OUT="$ROOT_DIR/help_output.txt"
$BIN -h > "$HELP_OUT"
if ! grep -q "Usage: Replace" "$HELP_OUT"; then
  echo "-h test failed: usage line not found"
  cat "$HELP_OUT"
  exit 2
fi

echo "Running functional replacement test..."
EXPECTED="$ROOT_DIR/fox-replace-dog-with-cat.txt"
INPUT="$ROOT_DIR/fox.txt"
OUTPUT="$ROOT_DIR/output.txt"
$BIN "$INPUT" "$OUTPUT" "dog" "cat"

# Normalize trailing final newlines for a robust comparison
# Compare files after removing only the final trailing newline (if present)
exp=$(printf "%s" "$(cat "$EXPECTED")")
out=$(printf "%s" "$(cat "$OUTPUT")")

if [ "$exp" != "$out" ]; then
  echo "Functional replacement test failed: output differs from expected"
  echo "--- Expected (trimmed) ---"
  printf "%s" "$exp"
  echo
  echo "--- Actual (trimmed) ---"
  printf "%s" "$out"
  echo
  exit 3
fi

echo "All tests passed."

run_case() {
  local search="$1" replace="$2" input="$3" expected="$4" out="$5"
  echo "Running case: search='${search}' replace='${replace}' input='${input}' -> out='${out}'"
  "$BIN" "$input" "$out" "$search" "$replace"
  exp=$(printf "%s" "$(cat "$expected")")
  outv=$(printf "%s" "$(cat "$out")")
  if [ "$exp" != "$outv" ]; then
    echo "Test failed for search='${search}' replace='${replace}'"
    echo "--- Expected (trimmed) ---"
    printf "%s" "$exp"
    echo
    echo "--- Actual (trimmed) ---"
    printf "%s" "$outv"
    echo
    exit 4
  fi
}

echo "Running example tests..."
# Example 1
run_case "ma" "mama" "$ROOT_DIR/ex1_input.txt" "$ROOT_DIR/ex1_expected.txt" "$ROOT_DIR/ex1_output.txt"
# Example 2
run_case "1231234" "XYZ" "$ROOT_DIR/ex2_input.txt" "$ROOT_DIR/ex2_expected.txt" "$ROOT_DIR/ex2_output.txt"
# Example 3: empty search string -> output unchanged
run_case "" "unused" "$ROOT_DIR/ex3_input.txt" "$ROOT_DIR/ex3_expected.txt" "$ROOT_DIR/ex3_output.txt"

# Example 4: missing replacement argument should cause non-zero exit
echo "Running example 4 (missing replacement)"
set +e
"$BIN" "$ROOT_DIR/ex1_input.txt" "$ROOT_DIR/ex1_maybe_output.txt" "hello"
rc=$?
set -e
if [ $rc -eq 0 ]; then
  echo "Example 4 failed: expected non-zero exit when replacement argument is missing"
  exit 5
fi

echo "All tests passed."
