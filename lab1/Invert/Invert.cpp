#include "exceptions.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string HELP_TEXT = "Usage: invert <input file>\n"
							  "If u don't provide an input file, the program will read from standard input line by line.\n"
							  "Proggram works with square matrices of 3x3 size. Matrix will be inverted and returned as output.\n";

using Matrix = std::vector<std::vector<double>>;
using VecD = std::vector<double>;

Matrix ReadMatrix(std::istream& input)
{
	Matrix matrix;
	std::string line;

	while (std::getline(input, line) && matrix.size() < 3)
	{
		std::istringstream iss(line);

		VecD row;
		double value;
		while (row.size() < 3 && (iss >> value))
		{
			row.push_back(value);
		}
		if (iss.fail() && !iss.eof())
		{
			throw InvalidMatrixException("Non-numeric value encountered in input");
		}
		matrix.push_back(row);
	}

	return matrix;
}

Matrix PrintMatrix(const Matrix& matrix, std::ostream& output)
{
	output << std::fixed << std::setprecision(3);
	for (const auto& row : matrix)
	{
		for (const auto& elem : row)
		{
			output << elem << "\t";
		}
		output << std::endl;
	}
	return matrix;
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

void ValidateMatrix(const Matrix& matrix)
{
	if (matrix.size() != 3)
	{
		throw InvalidMatrixFormatException("Expected 3 rows, got " + std::to_string(matrix.size()));
	}
	for (size_t i = 0; i < matrix.size(); ++i)
	{
		if (matrix[i].size() != 3)
		{
			throw InvalidMatrixFormatException("Row " + std::to_string(i) + " expected 3 columns, got " + std::to_string(matrix[i].size()));
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		std::cout << HELP_TEXT << std::endl;
		return 0;
	}

	Matrix matrix;

	if (argc == 2)
	{
		std::ifstream inputFile(argv[1]);
		if (!inputFile.is_open())
		{
			std::cerr << "Error: Could not open file " << argv[1] << std::endl;
			return 1;
		}
		try
		{
			matrix = ReadMatrix(inputFile);
		}
		catch (const InvalidMatrixException& e)
		{
			std::cerr << "Invalid matrix" << std::endl;
			return 0;
		}
	}
	else if (argc == 1)
	{
		try
		{
			matrix = ReadMatrix(std::cin);
		}
		catch (const InvalidMatrixException& e)
		{
			std::cerr << "Invalid matrix" << std::endl;
			return 0;
		}
	}
	else
	{
		std::cerr << "Error: Invalid number of arguments. Use -h for help." << std::endl;
		return 1;
	}

	try
	{
		ValidateMatrix(matrix);
		Matrix inverted = InvertMatrix(matrix);
		PrintMatrix(inverted, std::cout);
	}
	catch (const NonInvertibleMatrixException& e)
	{
		std::cerr << "Non-invertible" << std::endl;
	}
	catch (const InvalidMatrixFormatException& e)
	{
		std::cerr << "Invalid matrix format" << std::endl;
	}
	catch (const InvalidMatrixException& e)
	{
		std::cerr << "Invalid matrix" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
