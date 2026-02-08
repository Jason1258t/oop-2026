#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

// TODO: Add error handling (e.g., file open errors, argument parsing errors, etc.)

std::string ReplaceString(const std::string& subject,
	const std::string& searchString, const std::string& replacementString)
{
	if (searchString.empty())
	{
		return subject;
	}
	size_t pos = 0;

	std::string result;
	while (pos < subject.length())
	{
		size_t foundPos = subject.find(searchString, pos);
		if (foundPos != std::string::npos)
		{
			result.append(subject, pos, foundPos - pos);
			result.append(replacementString);
			pos = foundPos + searchString.length();
		}
		else
		{
			result.append(subject, pos, subject.length() - pos);
			break;
		}
	}
	return result;
}

void CopyStreamWithReplacement(std::istream& input, std::ostream& output,
	const std::string& searchString, const std::string& replacementString)
{
	if (searchString.empty())
	{
		output << input.rdbuf();
		return;
	}

	std::string buffer;
	char c;

	while (input.get(c))
	{
		buffer += c;

		if (buffer.size() >= searchString.size() && buffer.substr(buffer.size() - searchString.size()) == searchString)
		{
			buffer.erase(buffer.size() - searchString.size());
			output << buffer << replacementString;
			buffer.clear();
		}
	}

	output << buffer;
}

void CopyFileWithReplacement(const std::string& inputFilename, const std::string& outputFilename,
	const std::string& search, const std::string& replace)
{
	std::ifstream inputFile;
	inputFile.open(inputFilename);
	if (!inputFile.is_open())
	{
		throw std::runtime_error("Can't open file " + inputFilename);
	}
	std::ofstream outputFile;
	outputFile.open(outputFilename);
	if (!outputFile.is_open())
	{
		throw std::runtime_error("Can't open file " + outputFilename);
	}
	CopyStreamWithReplacement(inputFile, outputFile, search, replace);
	outputFile.flush();
}


// TODO: refactor main to separate argument parsing, file handling, and string replacement logic more cleanly (e.g., using helper functions, etc.)
int main(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		std::cout << "Usage: replace <inputFile> <outputFile> <searchString> <replacementString>" << std::endl;
		std::cout << "Replaces all occurrences of <searchString> with <replacementString> in <inputFile>." << std::endl;
		return 0;
	}
	std::string searchString, replacementString;
	if (argc == 5)
	{
		std::string searchString, replacementString, inputFile, outputFile;

		inputFile = argv[1];
		outputFile = argv[2];
		searchString = argv[3];
		replacementString = argv[4];

		CopyFileWithReplacement(inputFile, outputFile, searchString, replacementString);
	}
	else if (argc != 5)
	{

		std::cout << "Invalid argument count" << std::endl
				  << "Usage: replace <inputFile> <outputFile> <searchString> <replacementString>" << std::endl;

		std::string searchString, replacementString, subject;
		std::cout << "Enter: searchString replacementString subject " << std::endl;

		std::getline(std::cin, searchString);
		std::getline(std::cin, replacementString);

		std::string line;
		while (std::getline(std::cin, line))
		{
			if (!subject.empty())
			{
				subject += '\n';
			}
			subject += line;
		}

		// TODO: verify that all args were passed correctly (e.g., searchString and replacementString should not be empty, etc.)
		// Directly need to verify input interruption by EOF (e.g., Ctrl+D in Unix)

		std::cout << "Result: " << std::endl
				  << ReplaceString(subject, searchString, replacementString) << std::endl;
	}

	return 0;
}