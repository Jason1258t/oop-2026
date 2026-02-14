#include "Numbers.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>

TEST_CASE("ReadNumbers reads numbers from stream", "[ReadNumbers]")
{
	SECTION("Reads valid numbers")
	{
		std::stringstream ss("1.5 2.7 -3.2 10");
		std::vector<double> expected = { 1.5, 2.7, -3.2, 10.0 };

		REQUIRE(ReadNumbers(ss) == expected);
	}

	SECTION("Reads single number")
	{
		std::stringstream ss("42.5");
		std::vector<double> expected = { 42.5 };

		REQUIRE(ReadNumbers(ss) == expected);
	}

	SECTION("Reads empty input")
	{
		std::stringstream ss("");
		std::vector<double> expected = {};

		REQUIRE(ReadNumbers(ss) == expected);
	}

	SECTION("Throws on invalid input")
	{
		std::stringstream ss("1.5 abc 2.7");

		REQUIRE_THROWS_AS(ReadNumbers(ss), std::runtime_error);
		REQUIRE_THROWS_WITH(ReadNumbers(ss), "Bad value");
	}
}

TEST_CASE("ProcessNumbers adds average of positive elements", "[ProcessNumbers]")
{
	SECTION("Mixed positive and negative numbers")
	{
		std::vector<double> input = { 1.0, 2.0, 3.0, -4.0, -5.0 };
		// Положительные: 1, 2, 3; сумма = 6, среднее = 2
		// Результат: 1+2=3, 2+2=4, 3+2=5, -4+2=-2, -5+2=-3
		std::vector<double> expected = { 3.0, 4.0, 5.0, -2.0, -3.0 };

		auto result = ProcessNumbers(input);
		REQUIRE(result == expected);
	}

	SECTION("All positive numbers")
	{
		std::vector<double> input = { 10.0, 20.0, 30.0 };
		// Сумма = 60, среднее = 20
		// Результат: 10+20=30, 20+20=40, 30+20=50
		std::vector<double> expected = { 30.0, 40.0, 50.0 };

		auto result = ProcessNumbers(input);
		REQUIRE(result == expected);
	}

	SECTION("All negative numbers")
	{
		std::vector<double> input = { -1.0, -2.0, -3.0 };
		// Нет положительных элементов, возвращаем исходный вектор
		std::vector<double> expected = { -1.0, -2.0, -3.0 };

		auto result = ProcessNumbers(input);
		REQUIRE(result == expected);
	}

	SECTION("Mix including zero")
	{
		std::vector<double> input = { 0.0, 5.0, -5.0, 10.0 };
		// Положительные: 5, 10; сумма = 15, среднее = 7.5
		// Результат: 0+7.5=7.5, 5+7.5=12.5, -5+7.5=2.5, 10+7.5=17.5
		std::vector<double> expected = { 7.5, 12.5, 2.5, 17.5 };

		auto result = ProcessNumbers(input);
		REQUIRE(result == expected);
	}

	SECTION("Single positive number")
	{
		std::vector<double> input = { 42.0 };
		// Сумма = 42, среднее = 42
		// Результат: 42+42=84
		std::vector<double> expected = { 84.0 };

		auto result = ProcessNumbers(input);
		REQUIRE(result == expected);
	}

	SECTION("Single negative number")
	{
		std::vector<double> input = { -42.0 };
		// Нет положительных элементов
		std::vector<double> expected = { -42.0 };

		auto result = ProcessNumbers(input);
		REQUIRE(result == expected);
	}

	SECTION("Empty vector")
	{
		std::vector<double> input = {};
		std::vector<double> expected = {};

		auto result = ProcessNumbers(input);
		REQUIRE(result == expected);
	}
}

TEST_CASE("PrintSortedNumbers prints sorted numbers with precision 3", "[PrintSortedNumbers]")
{
	SECTION("Prints sorted numbers")
	{
		std::vector<double> input = { 3.14159, 2.71828, 1.41421 };
		std::stringstream ss;

		PrintSortedNumbers(input, ss);

		std::string expected = "1.414 2.718 3.142\n";
		REQUIRE(ss.str() == expected);
	}

	SECTION("Prints empty vector")
	{
		std::vector<double> input = {};
		std::stringstream ss;

		PrintSortedNumbers(input, ss);

		std::string expected = "\n";
		REQUIRE(ss.str() == expected);
	}

	SECTION("Prints single number")
	{
		std::vector<double> input = { 42.424242 };
		std::stringstream ss;

		PrintSortedNumbers(input, ss);

		std::string expected = "42.424\n";
		REQUIRE(ss.str() == expected);
	}

	SECTION("Does not modify original vector")
	{
		std::vector<double> input = { 3.0, 1.0, 2.0 };
		std::vector<double> original = input;
		std::stringstream ss;

		PrintSortedNumbers(input, ss);

		// Проверяем, что исходный вектор не изменился
		REQUIRE(input == original);
	}

	SECTION("Handles negative numbers correctly")
	{
		std::vector<double> input = { -5.0, 3.0, -1.0, 0.0 };
		std::stringstream ss;

		PrintSortedNumbers(input, ss);

		std::string expected = "-5.000 -1.000 0.000 3.000\n";
		REQUIRE(ss.str() == expected);
	}
}