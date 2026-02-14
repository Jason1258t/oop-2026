add_library(htmllib HtmlDecode.cpp)
target_include_directories(htmllib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(html-decode main.cpp)
target_link_libraries(html-decode PRIVATE htmllib)

add_subdirectory(tests)
