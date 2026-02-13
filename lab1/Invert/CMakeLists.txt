add_executable(invert main.cpp MatrixMath.cpp)
enable_testing()

set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex1.txt")
add_test(NAME InvertEx1 COMMAND invert ${TEST_INPUT_FILE})
set_tests_properties(InvertEx1 PROPERTIES PASS_REGULAR_EXPRESSION "-24.000\t18.000\t5.000\t\n20.000\t-15.000\t-4.000\t\n-5.000\t4.000\t1.000\t")

set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex2.txt")
add_test(NAME InvertEx2 COMMAND invert ${TEST_INPUT_FILE})
set_tests_properties(InvertEx2 PROPERTIES PASS_REGULAR_EXPRESSION "-0.009\t0.321\t0.131\t\n0.212\t-0.448\t0.017\t\n-0.194\t0.806\t-0.278")

set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex3_non-invertible.txt")
add_test(NAME InvertEx3NonInvertible COMMAND invert ${TEST_INPUT_FILE})
set_tests_properties(InvertEx3NonInvertible PROPERTIES PASS_REGULAR_EXPRESSION "Non-invertible")

set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/ex4_invalid_char.txt")
add_test(NAME InvertEx4InvalidChar COMMAND invert ${TEST_INPUT_FILE})
set_tests_properties(InvertEx4InvalidChar PROPERTIES PASS_REGULAR_EXPRESSION "^Invalid matrix\n$")

set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/invalid_format.txt")
add_test(NAME InvertEx5InvalidFormat COMMAND invert ${TEST_INPUT_FILE})
set_tests_properties(InvertEx5InvalidFormat PROPERTIES PASS_REGULAR_EXPRESSION "^Invalid matrix format\n$")

# TODO: check all use-cases and exceptions, add more tests