#include <fstream>
#include <iostream>
#include <string>

const std::string HELP_TEXT = "Usage:\n"
							  "To crypt data use: crypt <mode> <input file> <output file> <key>\n"
							  "Programm will read data from input file, crypt or decrypt it using provided key and write result to output file.\n"
							  "Parameters:\n"
							  "\t- <mode> should be either 'crypt' or 'decrypt'.\n"
							  "\t- <key> should be a number in range [0, 255].";

const std::string CRYPT_MODE = "crypt";
const std::string DECRYPT_MODE = "decrypt";

uint8_t ApplyKey(uint8_t byte, uint8_t key)
{
	return byte ^ key;
}

uint8_t ShakeByte(uint8_t byte)
{
	uint8_t newByte = byte & 0b11100111;
	newByte |= ((byte & 0b00001000) << 1);
	newByte |= ((byte & 0b00010000) >> 1);
	return newByte;
}

uint8_t CryptByte(uint8_t byte, uint8_t key)
{
	return ShakeByte(ApplyKey(byte, key));
}

uint8_t DecryptByte(uint8_t byte, uint8_t key)
{
	return ApplyKey(ShakeByte(byte), key);
}

void ValidateFiles(std::string inputFile, std::string outputFile, std::ifstream& in, std::ofstream& out)
{
	if (!in.is_open())
	{
		throw std::system_error(errno, std::generic_category(), "Failed to open file " + inputFile);
	}

	if (!out.is_open())
	{
		throw std::system_error(errno, std::generic_category(), "Failed to open file " + outputFile);
	}
}

void ProcessFiles(const std::string& inputFile, const std::string& outputFile, const uint8_t& key, const std::string& mode)
{
	std::ifstream in(inputFile, std::ios::binary);
	std::ofstream out(outputFile, std::ios::binary);
	
	ValidateFiles(inputFile, outputFile, in, out);

	char byte;
	while (in.get(byte))
	{
		char res = mode == CRYPT_MODE ? CryptByte(byte, key) : DecryptByte(byte, key);
		out.put(res);
	}
}

int main(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		std::cout << HELP_TEXT << std::endl;
		return 0;
	}
	if (argc != 5)
	{
		std::cerr << "Invalid number of arguments. Use -h for help." << std::endl;
		return 1;
	}

	std::string mode = argv[1];
	std::string inputFile = argv[2];
	std::string outputFile = argv[3];
	uint8_t key = std::stoi(argv[4]);

	if (mode != CRYPT_MODE && mode != DECRYPT_MODE)
	{
		std::cerr << "Invalid mode. Use -h for help." << std::endl;
		return 1;
	}

	ProcessFiles(inputFile, outputFile, key, mode);

	return 0;
}