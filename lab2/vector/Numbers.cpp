#include "Numbers.hpp"
#include <iomanip>
#include <numeric>

std::vector<double> ReadNumbers(std::istream& in)
{
	std::vector<double> nums;
	double n;

	while (in >> n)
	{
		nums.push_back(n);
	}

	if (in.fail() && !in.eof())
	{
		throw std::runtime_error("Bad value");
	}
	return nums;
}

std::vector<double> ProcessNumbers(const std::vector<double>& nums)
{
	std::vector<double> newNums;
	double sumPositive = std::accumulate(nums.begin(), nums.end(), 0.0,
		[](double acc, double n) { return n > 0 ? acc + n : acc; });

	int countPositive = std::count_if(nums.begin(), nums.end(), [](double n) { return n > 0; });

	if (countPositive == 0)
		return nums;

	double avg = sumPositive / countPositive;

	for (double n : nums)
	{
		newNums.push_back(n + avg);
	}
	return newNums;
}

void PrintSortedNumbers(const std::vector<double>& nums, std::ostream& out)
{
	std::vector<double> sorted = nums;
	std::sort(sorted.begin(), sorted.end());

	out << std::fixed << std::setprecision(3);
	for (size_t i = 0; i < sorted.size(); ++i)
	{
		if (i)
			out << " ";
		out << sorted[i];
	}
	out << std::endl;
}