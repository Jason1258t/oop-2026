#pragma once

#include <iostream>
#include <string>
#include <vector>

// - Функция ReadNumbers, которая считывает числа в vector.
// - Функция ProcessNumbers, которая обрабатывает переданный ей vector.
// - Функция PrintSortedNumbers, которая выводит переданный ей вектор в порядке неубывания.
// Эта функция не должна модифицировать переданный ей vector видимым для вызывающего кода образом.

std::vector<double> ReadNumbers(std::istream& in);

std::vector<double> ProcessNumbers(const std::vector<double>& nums);

void PrintSortedNumbers(const std::vector<double>& nums, std::ostream& out);