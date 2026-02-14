#include "Numbers.hpp"

int main(int argc, char* argv[])
{
	auto nums = ReadNumbers(std::cin);
	nums = ProcessNumbers(nums);
	PrintSortedNumbers(nums, std::cout);
}