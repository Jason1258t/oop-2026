#pragma once

#include <vector>

using Matrix = std::vector<std::vector<double>>;
using VecD = std::vector<double>;

double Determinant(const Matrix& matrix);
Matrix GetMinor(const Matrix& matrix, int row, int col);
Matrix AdjugateMatrix(const Matrix& matrix);
Matrix TransposeMatrix(const Matrix& matrix);
Matrix InvertMatrix(const Matrix& matrix);