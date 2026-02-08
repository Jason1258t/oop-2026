#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>


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

        if (buffer.size() >= searchString.size() &&
            buffer.substr(buffer.size() - searchString.size()) == searchString)
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

int main(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		std::cout << "Usage: replace <inputFile> <outputFile> <searchString> <replacementString>" << std::endl;
		std::cout << "Replaces all occurrences of <searchString> with <replacementString> in <inputFile>." << std::endl;
		return 0;
	}

	if (argc != 5)
	{
		std::cout << "Invalid argument count" << std::endl
				  << "Usage: replace <inputFile> <outputFile> <searchString> <replacementString>" << std::endl;

		return 1;
	}

	// Добавьте обработку ошибок

	CopyFileWithReplacement(argv[1], argv[2], argv[3], argv[4]);

	return 0;
}