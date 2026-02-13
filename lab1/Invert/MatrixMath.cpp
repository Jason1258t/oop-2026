#include "MatrixMath.hpp"
#include "Exceptions.hpp"
#include <string>

double Determinant(const Matrix& matrix)
{
	if (matrix.size() == 2)
	{
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	}
	double det = 0.0;
	for (size_t j = 0; j < matrix[0].size(); ++j)
	{
		det += (j % 2 == 0 ? 1 : -1) * matrix[0][j] * Determinant(GetMinor(matrix, 0, j));
	}
	return det;
}

Matrix GetMinor(const Matrix& matrix, int row, int col)
{
	Matrix minor;
	for (size_t i = 0; i < matrix.size(); ++i)
	{
		if (i == row)
			continue;
		VecD minorRow;
		for (size_t j = 0; j < matrix[i].size(); ++j)
		{
			if (j == col)
				continue;
			minorRow.push_back(matrix[i][j]);
		}
		minor.push_back(minorRow);
	}
	return minor;
}

Matrix AdjugateMatrix(const Matrix& matrix)
{
	Matrix adjMatrix(matrix.size(), VecD(matrix[0].size()));
	for (size_t i = 0; i < matrix.size(); ++i)
	{
		for (size_t j = 0; j < matrix[i].size(); ++j)
		{
			adjMatrix[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * Determinant(GetMinor(matrix, i, j));
		}
	}
	return adjMatrix;
}

Matrix TransposeMatrix(const Matrix& matrix)
{
	Matrix transposed(matrix[0].size(), VecD(matrix.size()));
	for (size_t i = 0; i < matrix.size(); ++i)
	{
		for (size_t j = 0; j < matrix[i].size(); ++j)
		{
			transposed[j][i] = matrix[i][j];
		}
	}
	return transposed;
}

Matrix InvertMatrix(const Matrix& matrix)
{
	double det = Determinant(matrix);
	if (det == 0)
	{
		throw NonInvertibleMatrixException();
	}

	Matrix adjugate = AdjugateMatrix(matrix);
	Matrix transposed = TransposeMatrix(adjugate);

	Matrix inverted(matrix.size(), VecD(matrix[0].size()));
	for (size_t i = 0; i < transposed.size(); ++i)
	{
		for (size_t j = 0; j < transposed[i].size(); ++j)
		{
			inverted[i][j] = transposed[i][j] / det;
		}
	}
	return inverted;
}
