add_executable(Replace Replace.cpp)

# Test files (paths are relative to this directory)
set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/fox.txt")
set(TEST_EXPECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/fox-replace-dog-with-cat.txt")
set(TEST_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/output.txt")

enable_testing()

# Тест 1: Проверка ошибки при запуске без аргументов
add_test(NAME ReplaceEmptyArgs COMMAND Replace)
set_tests_properties(ReplaceEmptyArgs PROPERTIES WILL_FAIL TRUE)

# Тест 2: Замена dog на cat
add_test(NAME ReplaceDogWithCat COMMAND Replace
    "${TEST_INPUT_FILE}"
    "${TEST_OUTPUT_FILE}"
    "dog" "cat"
)

# Test 3: Help output should contain usage
add_test(NAME ReplaceHelp COMMAND Replace -h)
set_tests_properties(ReplaceHelp PROPERTIES PASS_REGULAR_EXPRESSION "Usage: Replace")

# Test 4: Compare results using helper script that normalizes trailing newline
add_test(NAME CompareResultNormalized COMMAND
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/normalize_and_diff.sh"
    "${TEST_OUTPUT_FILE}" "${TEST_EXPECTED_FILE}")

# Ensure CompareResultNormalized runs after ReplaceDogWithCat
set_tests_properties(CompareResultNormalized PROPERTIES DEPENDS ReplaceDogWithCat)
