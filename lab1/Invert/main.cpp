#include "Exceptions.hpp"
#include "MatrixMath.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string HELP_TEXT = "Usage: invert <input file>\n"
							  "If u don't provide an input file, the program will read from standard input line by line.\n"
							  "Proggram works with square matrices of 3x3 size. Matrix will be inverted and returned as output.\n";

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

enum class ProgrammMode
{
	HELP,
	FILE,
	STDIN,
	INVALID
};

struct ProgrammArgs
{
	ProgrammMode mode;
	std::string inputFile;
};

ProgrammArgs ParseArguments(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		return { ProgrammMode::HELP };
	}
	else if (argc == 2)
	{
		return { ProgrammMode::FILE, argv[1] };
	}
	else if (argc == 1)
	{
		return { ProgrammMode::STDIN };
	}
	else
	{
		return { ProgrammMode::INVALID };
	}
}

Matrix GetMatrix(const ProgrammArgs& args)
{
	Matrix matrix;

	if (args.mode == ProgrammMode::FILE)
	{
		std::ifstream inputFile(args.inputFile);
		if (!inputFile.is_open())
		{
			throw std::runtime_error("Error: Could not open file " + args.inputFile);
		}
		matrix = ReadMatrix(inputFile);
	}
	else if (args.mode == ProgrammMode::STDIN)
	{
		matrix = ReadMatrix(std::cin);
	}
	else
	{
		throw std::runtime_error("Invalid program mode");
	}

	return matrix;
}

int main(int argc, char* argv[])
{
	auto args = ParseArguments(argc, argv);

	if (args.mode == ProgrammMode::INVALID)
	{
		std::cerr << "Invalid arguments. Use -h for help." << std::endl;
		return 1;
	}
	else if (args.mode == ProgrammMode::HELP)
	{
		std::cout << HELP_TEXT << std::endl;
		return 0;
	}

	try
	{
		Matrix matrix = GetMatrix(args);
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
