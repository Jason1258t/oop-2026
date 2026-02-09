#include <iostream>
#include <string>
#include <cstdint>

const std::string HELP_TEXT = "Usage: radix <source notation> <destination notation> <value>\n"
							  "Converts the given value from the source notation to the destination notation.\n"
                              "Proggram supports values in the range of 32-bit signed integers and notations from 2 to 36.\n"
							  "Example: radix.exe 10 16 255";

int32_t StringToInt(const std::string& str, int32_t radix)
{
	int32_t value = 0;
    int32_t sign = 1;
	for (char c : str)
	{
		int32_t digitValue;
        if (c == '-' && value == 0) {
            sign = -1;
            continue;
        }
		if (c >= '0' && c <= '9')
		{
			digitValue = c - '0';
		}
		else if (c >= 'A' && c <= 'Z')
		{
			digitValue = c - 'A' + 10;
		}
		else if (c >= 'a' && c <= 'z')
		{
			digitValue = c - 'a' + 10;
		}
		else
		{
			throw std::invalid_argument("Invalid character in input string: " + std::string(1, c));
		}

		if (digitValue >= radix)
		{
			throw std::invalid_argument("Digit value exceeds radix: " + std::to_string(digitValue) + " >= " + std::to_string(radix - 1));
		}

		value = value * radix + digitValue;
	}

	return value * sign;
}

std::string IntToString(int32_t n, int32_t radix)
{
    int64_t n64 = n; 
	if (n == 0)
	{
		return "0";
	}
    std::string sign = "";
    
    if (n64 < 0) {
        sign = "-";
        n64 = -n64;
    }
	std::string result = "";
	while (n64 > 0)
	{
		int32_t digit = n64 % radix;
		char digitChar;
		if (digit < 10)
		{
			digitChar = '0' + digit;
		}
		else
		{
			digitChar = 'A' + digit - 10;
		}
		result = digitChar + result;
		n64 /= radix;
	}

	return sign + result;
}

int main(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		std::cout << HELP_TEXT << std::endl;
		return 0;
	}

	if (argc != 4)
	{
		std::cerr << "Invalid arguments. Use -h for help." << std::endl;
		return 1;
	}

	int32_t sourceBase = std::stoi(argv[1]);
	int32_t destBase = std::stoi(argv[2]);
	std::string value = argv[3];

	try
	{
		std::cout << IntToString(StringToInt(value, sourceBase), destBase) << std::endl;
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 0;
}