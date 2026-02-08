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

add_test(NAME CompareDogWithCat COMMAND
    ${CMAKE_COMMAND} -E compare_files "${TEST_OUTPUT_FILE}" "${TEST_EXPECTED_FILE}")
set_tests_properties(CompareDogWithCat PROPERTIES DEPENDS ReplaceDogWithCat)

# Example tests from assignment
set(TEST_EX1_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex1_input.txt")
set(TEST_EX1_EXPECTED "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex1_expected.txt")
set(TEST_EX1_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ex1_output.txt")
add_test(NAME ReplaceEx1Run COMMAND Replace "${TEST_EX1_INPUT}" "${TEST_EX1_OUTPUT}" "ma" "mama")
add_test(NAME ReplaceEx1Compare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_EX1_OUTPUT}" "${TEST_EX1_EXPECTED}")
set_tests_properties(ReplaceEx1Compare PROPERTIES DEPENDS ReplaceEx1Run)

set(TEST_EX2_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex2_input.txt")
set(TEST_EX2_EXPECTED "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex2_expected.txt")
set(TEST_EX2_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ex2_output.txt")
add_test(NAME ReplaceEx2Run COMMAND Replace "${TEST_EX2_INPUT}" "${TEST_EX2_OUTPUT}" "1231234" "XYZ")
add_test(NAME ReplaceEx2Compare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_EX2_OUTPUT}" "${TEST_EX2_EXPECTED}")
set_tests_properties(ReplaceEx2Compare PROPERTIES DEPENDS ReplaceEx2Run)

# Example 3 (empty search string) will be tested via shell scripts (hard to pass empty arg portably in CTest)

# Test 3: Help output should contain usage
add_test(NAME ReplaceHelp COMMAND Replace -h)
set_tests_properties(ReplaceHelp PROPERTIES PASS_REGULAR_EXPRESSION "Usage: Replace")
