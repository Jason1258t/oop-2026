# compare_normalized.cmake
# Usage: cmake -Dactual=<actual-file> -Dexpected=<expected-file> -P compare_normalized.cmake
if(NOT DEFINED actual)
  message(FATAL_ERROR "compare_normalized.cmake requires -Dactual and -Dexpected variables set")
endif()
if(NOT DEFINED expected)
  message(FATAL_ERROR "compare_normalized.cmake requires -Dactual and -Dexpected variables set")
endif()


# Strip surrounding quotes if CTest/COMMAND passed quoted values
string(REPLACE '"' "" actual_file "${actual}")
string(REPLACE '"' "" expected_file "${expected}")

message(STATUS "compare_normalized: actual='${actual_file}' expected='${expected_file}'")

if(NOT EXISTS "${actual_file}")
  message(FATAL_ERROR "Actual file does not exist: ${actual_file}")
endif()
if(NOT EXISTS "${expected_file}")
  message(FATAL_ERROR "Expected file does not exist: ${expected_file}")
endif()

# Read files
file(READ "${actual_file}" actual_content)
file(READ "${expected_file}" expected_content)

# Remove a single trailing newline (\n) if present
string(REGEX REPLACE "\n$" "" actual_trimmed "${actual_content}")
string(REGEX REPLACE "\n$" "" expected_trimmed "${expected_content}")

if(NOT "${actual_trimmed}" STREQUAL "${expected_trimmed}")
  message(STATUS "--- Expected (trimmed) ---")
  message(STATUS "${expected_trimmed}")
  message(STATUS "--- Actual (trimmed) ---")
  message(STATUS "${actual_trimmed}")
  message(FATAL_ERROR "Files differ after normalizing final newline")
endif()

message(STATUS "Files match after normalizing final newline")
