add_executable(radix Radix.cpp)
enable_testing()

add_test(NAME ExampleHexMax COMMAND radix 10 16 255)
set_tests_properties(ExampleHexMax PROPERTIES PASS_REGULAR_EXPRESSION "FF")

add_test(NAME ExampleHexMin COMMAND radix 10 16 0)
set_tests_properties(ExampleHexMin PROPERTIES PASS_REGULAR_EXPRESSION "0")

add_test(NAME ExampleBinaryMax COMMAND radix 10 2 256)
set_tests_properties(ExampleBinaryMax PROPERTIES PASS_REGULAR_EXPRESSION "100000000")

add_test(NAME InvalidCharacter COMMAND radix 10 16 "1?")
set_tests_properties(InvalidCharacter PROPERTIES PASS_REGULAR_EXPRESSION "ERROR: ")

add_test(NAME InvalidRadix COMMAND radix 2 10 123)
set_tests_properties(InvalidRadix PROPERTIES PASS_REGULAR_EXPRESSION "ERROR: ")

add_test(NAME NegativeOct COMMAND radix 10 8 -16)
set_tests_properties(NegativeOct PROPERTIES PASS_REGULAR_EXPRESSION "-20")

add_test(NAME ExampleTest COMMAND radix 16 10 1F)
set_tests_properties(ExampleTest PROPERTIES PASS_REGULAR_EXPRESSION "31")

set(INT32_MAX "2147483647")
set(INT32_MIN "-2147483648")

add_test(NAME Int32_MaxPositive_Hex COMMAND radix 10 16 ${INT32_MAX})
set_tests_properties(Int32_MaxPositive_Hex PROPERTIES PASS_REGULAR_EXPRESSION "7FFFFFFF")

add_test(NAME Int32_NearMax_Hex COMMAND radix 10 16 2147483646)
set_tests_properties(Int32_NearMax_Hex PROPERTIES PASS_REGULAR_EXPRESSION "7FFFFFFE")

add_test(NAME Int32_MinNegative_Hex COMMAND radix 10 16 ${INT32_MIN})
set_tests_properties(Int32_MinNegative_Hex PROPERTIES PASS_REGULAR_EXPRESSION "-80000000")