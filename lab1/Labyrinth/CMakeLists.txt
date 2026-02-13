add_executable(labyrinth main.cpp WaveAlgorithm.cpp)
enable_testing()

set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/labyrinth-1.txt")
set(TEST_EXPECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/labyrinth-1-expected.txt")
set(TEST_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/output-1.txt")
add_test(NAME LabyrinthEx1 COMMAND labyrinth ${TEST_INPUT_FILE} ${TEST_OUTPUT_FILE})
add_test(NAME LabyrinthEx1Compare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_OUTPUT_FILE}" "${TEST_EXPECTED_FILE}")
set_tests_properties(LabyrinthEx1Compare PROPERTIES DEPENDS LabyrinthEx1)


set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/labyrinth-2.txt")
set(TEST_EXPECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/labyrinth-2-expected.txt")
set(TEST_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/output-2.txt")
add_test(NAME LabyrinthEx2 COMMAND labyrinth ${TEST_INPUT_FILE} ${TEST_OUTPUT_FILE})
add_test(NAME LabyrinthEx2Compare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_OUTPUT_FILE}" "${TEST_EXPECTED_FILE}")
set_tests_properties(LabyrinthEx2Compare PROPERTIES DEPENDS LabyrinthEx2)


set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/labyrinth-3.txt")
set(TEST_EXPECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/labyrinth-3-expected.txt")
set(TEST_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/output-3.txt")
add_test(NAME LabyrinthEx3 COMMAND labyrinth ${TEST_INPUT_FILE} ${TEST_OUTPUT_FILE})
add_test(NAME LabyrinthEx3Compare COMMAND ${CMAKE_COMMAND} -E compare_files "${TEST_OUTPUT_FILE}" "${TEST_EXPECTED_FILE}")
set_tests_properties(LabyrinthEx3Compare PROPERTIES DEPENDS LabyrinthEx3)

# TODO: repare test 2, add tests for more use-cases