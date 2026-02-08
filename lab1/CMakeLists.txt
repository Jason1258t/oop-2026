# Создаем исполняемый файл
add_executable(Replace Replace/Replace.cpp)

# Включаем возможность тестирования
enable_testing()

# Описываем тесты (аналоги скрипта из второго скриншота)
# Тест 1: Проверка ошибки при запуске без аргументов
add_test(NAME ReplaceEmptyArgs COMMAND Replace)
set_tests_properties(ReplaceEmptyArgs PROPERTIES WILL_FAIL TRUE)

# Тест 2: Замена dog на cat
add_test(NAME ReplaceDogWithCat COMMAND Replace 
    "${CMAKE_CURRENT_SOURCE_DIR}/Replace/tests/fox.txt" 
    "${CMAKE_CURRENT_BINARY_DIR}/output.txt" 
    "dog" "cat"
)

# Мы используем встроенную команду cmake -E compare_files
add_test(NAME CompareResult COMMAND ${CMAKE_COMMAND} -E compare_files
    "${CMAKE_CURRENT_BINARY_DIR}/output.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/Replace/tests/fox-replace-dog-with-cat.txt"
)