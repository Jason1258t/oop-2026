cmake_minimum_required(VERSION 3.20) # Актуальная версия CMake[citation:1]
project(CppCourse LANGUAGES CXX)      # Название проекта и язык (C++)

# Установка стандарта C++ (в вашем случае Apple Clang 17 поддерживает C++20/23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # Обязательное использование стандарта[citation:5]
# Опционально: можно явно указать компилятор, если нужно
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")[citation:1]

# Включение всех подпроектов (лабораторных)
add_subdirectory(lab1)

# ... добавьте add_subdirectory для lab3, lab4 и т.д.