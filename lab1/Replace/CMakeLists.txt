add_executable(replace Replace.cpp)
enable_testing()

# Test files (paths are relative to this directory)
add_test(NAME ReplaceEmptyArgs COMMAND replace)
set_tests_properties(ReplaceEmptyArgs PROPERTIES WILL_FAIL TRUE)

# Тест 2: Замена dog на cat
set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/fox.txt")
set(TEST_EXPECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/fox-replace-dog-with-cat.txt")
set(TEST_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/output.txt")
add_test(NAME ReplaceDogWithCat COMMAND replace "${TEST_INPUT_FILE}" "${TEST_OUTPUT_FILE}" "dog" "cat")
add_test(NAME CompareDogWithCat COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_OUTPUT_FILE}" "${TEST_EXPECTED_FILE}")
set_tests_properties(CompareDogWithCat PROPERTIES DEPENDS ReplaceDogWithCat)

set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/fox.txt")
set(TEST_EXPECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/fox.txt")
set(TEST_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/output.txt")
add_test(NAME ReplaceEmptySearch COMMAND replace "${TEST_INPUT_FILE}" "${TEST_OUTPUT_FILE}" "" "cat")
add_test(NAME CompareEmptySearch COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_OUTPUT_FILE}" "${TEST_EXPECTED_FILE}")
set_tests_properties(CompareEmptySearch PROPERTIES DEPENDS ReplaceEmptySearch)

set(TEST_EX1_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex1_input.txt")
set(TEST_EX1_EXPECTED "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex1_expected.txt")
set(TEST_EX1_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ex1_output.txt")
add_test(NAME ReplaceEx1Run COMMAND replace "${TEST_EX1_INPUT}" "${TEST_EX1_OUTPUT}" "ma" "mama")
add_test(NAME ReplaceEx1Compare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_EX1_OUTPUT}" "${TEST_EX1_EXPECTED}")
set_tests_properties(ReplaceEx1Compare PROPERTIES DEPENDS ReplaceEx1Run)

set(TEST_EX2_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex2_input.txt")
set(TEST_EX2_EXPECTED "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex2_expected.txt")
set(TEST_EX2_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ex2_output.txt")
add_test(NAME ReplaceEx2Run COMMAND replace "${TEST_EX2_INPUT}" "${TEST_EX2_OUTPUT}" "1231234" "XYZ")
add_test(NAME ReplaceEx2Compare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_EX2_OUTPUT}" "${TEST_EX2_EXPECTED}")
set_tests_properties(ReplaceEx2Compare PROPERTIES DEPENDS ReplaceEx2Run)


set(TEST_EMPTY_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/tests/empty.txt")
set(TEST_EMPTY_EXPECTED "${CMAKE_CURRENT_SOURCE_DIR}/tests/empty.txt")
set(TEST_EMPTY_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/empty_output.txt")
add_test(NAME ReplaceEmptyRun COMMAND replace "${TEST_EMPTY_INPUT}" "${TEST_EMPTY_OUTPUT}" "1231234" "XYZ")
add_test(NAME ReplaceEmptyCompare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_EMPTY_OUTPUT}" "${TEST_EMPTY_EXPECTED}")
set_tests_properties(ReplaceEx2Compare PROPERTIES DEPENDS ReplaceEx2Run)

set(TEST_REPLACE_WITH_ENDL_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/tests/replce_with_endl.txt") 
set(TEST_REPLACE_WITH_ENDL_EXPECTED "${CMAKE_CURRENT_SOURCE_DIR}/tests/replace_with_endl_expectet.txt") 
set(TEST_REPLACE_WITH_ENDL_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/replace_with_endl_output.txt") 
add_test(NAME ReplaceWithEndlRun COMMAND replace "${TEST_REPLACE_WITH_ENDL_INPUT}" "${TEST_REPLACE_WITH_ENDL_OUTPUT}" "line\n" "") 
add_test(NAME ReplaceWithEndlCompare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_REPLACE_WITH_ENDL_OUTPUT}" "${TEST_REPLACE_WITH_ENDL_EXPECTED}") 
set_tests_properties(ReplaceWithEndlCompare PROPERTIES DEPENDS ReplaceWithEndlRun)

add_test(NAME ReplaceHelp COMMAND replace -h)
set_tests_properties(ReplaceHelp PROPERTIES PASS_REGULAR_EXPRESSION "Usage: replace")
