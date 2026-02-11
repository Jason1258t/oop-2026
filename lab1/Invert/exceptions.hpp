#pragma once

#include <stdexcept>
#include <string>

class NonInvertibleMatrixException : public std::runtime_error
{
public:
	explicit NonInvertibleMatrixException(const std::string& message = "Matrix is singular and cannot be inverted") : std::runtime_error(message) {}
};

class InvalidMatrixException : public std::runtime_error
{
public:
	explicit InvalidMatrixException(const std::string& message = "Invalid matrix") : std::runtime_error(message) {}
};

class InvalidMatrixFormatException : public std::runtime_error
{
public:
	explicit InvalidMatrixFormatException(const std::string& message = "Invalid matrix format") : std::runtime_error(message) {}
};