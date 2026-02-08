cmake_minimum_required(VERSION 3.20)
project(CppCourse LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Критически важные настройки для тестирования
enable_testing()  # ✅ ДОБАВЬТЕ ЭТО! Без этого CTest не работает

# Настройка каталога сборки (опционально, но рекомендуется)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Включение всех подпроектов (лабораторных)
add_subdirectory(lab1)

# Добавьте для других лабораторных, когда они появятся
# add_subdirectory(lab2)
# add_subdirectory(lab3)

# 🔧 Создаем удобные цели для тестирования
add_custom_target(run-all-tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Запуск всех тестов"
    VERBATIM
)

# Цели для тестов конкретных лабораторных работ
add_custom_target(test-lab1
    COMMAND ${CMAKE_CTEST_COMMAND} --tests-regex ".*" --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lab1
    COMMENT "Запуск тестов lab1"
    VERBATIM
)

file(WRITE "${CMAKE_BINARY_DIR}/DartConfiguration.tcl"
    "# Minimal Dart configuration for CTest\n"
    "Site: \n"
    "BuildName: \n"
)

# Функция для удобного добавления тестовых целей
# function(add_lab_test_target lab_name)
#     add_custom_target(test-${lab_name}
#         COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${lab_name}
#         COMMENT "Запуск тестов ${lab_name}"
#         VERBATIM
#     )
# endfunction()

# Использование функции
# add_lab_test_target(lab1)
# add_lab_test_target(lab2)  # когда добавите lab2