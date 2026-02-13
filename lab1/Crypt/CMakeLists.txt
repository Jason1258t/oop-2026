add_executable(crypt Crypt.cpp)
enable_testing()

set(TEST_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.bin")
add_test(NAME CryptTest1 COMMAND crypt crypt ${TEST_INPUT_FILE} "${CMAKE_CURRENT_BINARY_DIR}/test1.enc" 170)
add_test(NAME CryptTest1Dec COMMAND crypt decrypt "${CMAKE_CURRENT_BINARY_DIR}/test1.enc" "${CMAKE_CURRENT_BINARY_DIR}/test1_dec.bin" 170)
add_test(NAME CryptTest1Compare COMMAND ${CMAKE_COMMAND} -E compare_files "${CMAKE_CURRENT_BINARY_DIR}/test1_dec.bin" "${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.bin")
set_tests_properties(CryptTest1Dec PROPERTIES DEPENDS CryptTest1)
set_tests_properties(CryptTest1Compare PROPERTIES DEPENDS CryptTest1Dec)

# TODO: add more tests, check edge cases, invalid files, etc.