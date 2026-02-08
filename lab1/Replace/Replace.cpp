#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

// Возвращает результат замены всех вхождения строки searchString внутри строки subject на replacementString
// Если строка searchString пустая, то возвращается subject
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
		std::cout << "Usage: Replace <inputFile> <outputFile> <searchString> <replacementString>\n";
		std::cout << "Replaces all occurrences of <searchString> with <replacementString> in <inputFile>.\n";
		return 0;
	}

	if (argc != 5)
	{
		std::cout << "Invalid argument count\n"
				  << "Usage: replace.exe <inputFile> <outputFile> <searchString> <replacementString>\n";

		std::cout << "Passed arguments:\n"
				  << argc << std::endl;
		return 1;
	}

	// Самостоятельно выделите код копирования содержимого файла в отдельную функцию CopyFileWithReplacement,
	// принимающую имена файлов, а также строки для поиска и замены
	// Добавьте обработку ошибок

	CopyFileWithReplacement(argv[1], argv[2], argv[3], argv[4]);

	return 0;
}