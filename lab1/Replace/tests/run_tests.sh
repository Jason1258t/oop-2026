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
