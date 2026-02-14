add_executable(test_vector tests.cpp)

target_link_libraries(test_vector
    PRIVATE
        Catch2::Catch2WithMain
        vectorlib
)

include(CTest)
include(Catch)
catch_discover_tests(test_vector)
