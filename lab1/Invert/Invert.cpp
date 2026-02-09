#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string HELP_TEXT = "Usage: invert <input file>\n"
							  "If u don't provide an input file, the program will read from standard input line by line.\n"
							  "Proggram works with square matrices of 3x3 size. Matrix will be inverted and returned as output.\n"
							  "Example: invert input.txt\n";

using Matrix = std::vector<std::vector<double>>;

Matrix ReadMatrix(std::istream& input)
{
	Matrix matrix;
	std::string line;
	int rowCount = 0;
	while (std::getline(input, line) && rowCount < 3)
	{
		int colCount = 0;
		std::vector<double> row;
		std::istringstream iss(line);
		std::string cell;
		while (std::getline(iss, cell, '\t') && colCount < 3)
		{
			row.push_back(std::stod(cell));
			colCount++;
		}
		matrix.push_back(row);
		rowCount++;
	}
	return matrix;
}

Matrix PrintMatrix(const Matrix& matrix, std::ostream& output)
{
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
		std::vector<double> minor_row;
		for (size_t j = 0; j < matrix[i].size(); ++j)
		{
			if (j == col)
				continue;
			minor_row.push_back(matrix[i][j]);
		}
		minor.push_back(minor_row);
	}
	return minor;
}

double Determinant2(const Matrix& matrix)
{
	return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

double Determinant(const Matrix& matrix)
{
	if (matrix.size() == 2)
	{
		return Determinant2(matrix);
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
	Matrix adjMatrix(matrix.size(), std::vector<double>(matrix[0].size()));
	for (size_t i = 0; i < matrix.size(); ++i)
	{
		for (size_t j = 0; j < matrix[i].size(); ++j)
		{
			adjMatrix[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * Determinant(GetMinor(matrix, i, j));
		}
	}
	return adjMatrix;
}

Matrix TransposeMatrix(const Matrix& matrix)
{
	Matrix transposed(matrix[0].size(), std::vector<double>(matrix.size()));
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
		throw std::runtime_error("Matrix is singular and cannot be inverted.");
	}

	Matrix adjugate = AdjugateMatrix(matrix);
	Matrix transposed = TransposeMatrix(adjugate);

	Matrix inverted(matrix.size(), std::vector<double>(matrix[0].size()));

	for (size_t i = 0; i < transposed.size(); ++i)
	{
		for (size_t j = 0; j < transposed[i].size(); ++j)
		{
			inverted[i][j] = transposed[i][j] / det;
		}
	}
	return inverted;
}

int main(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		std::cout << HELP_TEXT << std::endl;
		return 0;
	}

	std::istream* input;

	if (argc == 2)
	{
		std::ifstream inputFile(argv[1]);
		if (!inputFile.is_open())
		{
			std::cerr << "Error: Could not open file " << argv[1] << std::endl;
			return 1;
		}
		input = &inputFile;
	}
	else
	{
		input = &std::cin;
	}
	Matrix matrix = ReadMatrix(*input);
	PrintMatrix(matrix, std::cout);
	try
	{
		Matrix inverted = InvertMatrix(matrix);
		PrintMatrix(inverted, std::cout);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}