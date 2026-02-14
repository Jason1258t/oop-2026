add_executable(test_html tests.cpp)

target_link_libraries(test_html
    PRIVATE
        Catch2::Catch2WithMain
        htmllib
)

include(CTest)
include(Catch)
catch_discover_tests(test_html)