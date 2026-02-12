#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

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

int CopyFileWithReplacement(const std::string& inputFilename, const std::string& outputFilename,
	const std::string& search, const std::string& replace)
{
	std::ifstream inputFile;
	inputFile.open(inputFilename);
	if (!inputFile.is_open())
	{
		std::cerr << "Can't open input file" << std::endl;
		return 1;
	}
	std::ofstream outputFile;
	outputFile.open(outputFilename);
	if (!outputFile.is_open())
	{
		std::cerr << "Can't open output file" << std::endl;
		return 1;
	}
	CopyStreamWithReplacement(inputFile, outputFile, search, replace);
	outputFile.flush();
	return 0;
}

bool ReadInput(std::string& searchString, std::string& replacementString, std::string& subject)
{
	if (!std::getline(std::cin, searchString) || !std::getline(std::cin, replacementString))
	{
		return false;
	}

	std::string line;
	while (std::getline(std::cin, line))
	{
		if (!subject.empty())
		{
			subject += '\n';
		}
		subject += line;
	}

	return !subject.empty();
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
	std::string outputFile;
	std::string searchString;
	std::string replacementString;
};

ProgrammArgs ParseArguments(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		return { ProgrammMode::HELP };
	}
	else if (argc == 5)
	{
		return { ProgrammMode::FILE, argv[1], argv[2], argv[3], argv[4] };
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

int ProcessUserInput()
{
	std::string searchString, replacementString, subject;

	if (!ReadInput(searchString, replacementString, subject))
	{
		std::cerr << "ERROR" << std::endl;
		return 1;
	}

	std::cout << "Result: " << std::endl
			  << ReplaceString(subject, searchString, replacementString) << std::endl;

	return 0;
}

int main(int argc, char* argv[])
{
	ProgrammArgs args = ParseArguments(argc, argv);

	switch (args.mode)
	{
	case ProgrammMode::HELP:
		std::cout << "Usage: replace <inputFile> <outputFile> <searchString> <replacementString>" << std::endl
				  << "Replaces all occurrences of <searchString> with <replacementString> in <inputFile>." << std::endl;
		return 0;
	case ProgrammMode::FILE: {
		int result = CopyFileWithReplacement(args.inputFile, args.outputFile, args.searchString, args.replacementString);
		if (result != 0)
		{
			std::cerr << "ERROR" << std::endl;
			return 1;
		}
		return 0;
	}
	case ProgrammMode::STDIN:
		return ProcessUserInput();
	case ProgrammMode::INVALID:
	default:
		std::cout << "Invalid arguments. Use -h for help." << std::endl
				  << "Usage: replace <inputFile> <outputFile> <searchString> <replacementString>" << std::endl;
		return 1;
	}

	return 0;
}
