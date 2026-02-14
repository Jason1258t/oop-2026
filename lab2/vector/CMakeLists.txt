add_library(vectorlib Numbers.cpp)
target_include_directories(vectorlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(vec main.cpp)
target_link_libraries(vec PRIVATE vectorlib)

add_subdirectory(tests)
