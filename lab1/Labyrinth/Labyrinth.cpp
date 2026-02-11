#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using Labyrinth = std::vector<std::vector<char>>;
using RoadMap = std::vector<std::vector<int>>;
const int MAX_SIZE = 100;
using Position = std::pair<int, int>;

const std::string HELP_TEXT = "Usage: labyrinth <input file> <output file>\n"
							  "If u don't provide an input file, the program will read from stdin and out into stdout.\n"
							  "Proggram works with labyrinths represented as a grid of characters. The program will find the shortest path from 'A' to 'B' and mark it with '.'.\n"
							  "Maximum labyrinth size is 100x100. Allowed characters in labyrinth are: 'A', 'B', '#'. \n";

const std::vector<std::pair<int, int>> DIRECTIONS = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

enum class InputMode
{
	STDIN,
	FILE
};

Labyrinth ReadLabyrinth(std::istream& input)
{
	Labyrinth labyrinth;
	std::string line;

	while (std::getline(input, line) && labyrinth.size() < MAX_SIZE)
	{
		if (line.size() > MAX_SIZE)
		{
			throw std::runtime_error("Labyrinth row exceeds maximum allowed size of " + std::to_string(MAX_SIZE));
		}
		labyrinth.push_back(std::vector<char>(line.begin(), line.end()));
	}
	if (labyrinth.size() > MAX_SIZE)
	{
		throw std::runtime_error("Labyrinth exceeds maximum allowed size of " + std::to_string(MAX_SIZE) + " rows");
	}

	return labyrinth;
}

void PrintLabyrinth(const Labyrinth& labyrinth, std::ostream& output)
{
	for (const auto& row : labyrinth)
	{
		for (const auto& cell : row)
		{
			output << cell;
		}
		output << std::endl;
	}
}

Position GetCharPosition(const Labyrinth& labyrinth, const char& ch)
{
	// Fixed: Initialize loop variables and fix condition
	for (size_t y = 0; y < labyrinth.size(); y++)
	{
		for (size_t x = 0; x < labyrinth[y].size(); x++) // Changed > to <
		{
			if (labyrinth[y][x] == ch)
			{
				return { static_cast<int>(y), static_cast<int>(x) };
			}
		}
	}
	return { -1, -1 };
}

RoadMap WaveAlgorithm(const Labyrinth& labyrinth)
{
	Position a = GetCharPosition(labyrinth, 'A');
	Position b = GetCharPosition(labyrinth, 'B');

	if (a.first == -1 || a.second == -1)
	{
		throw std::runtime_error("Start position 'A' not found");
	}
	if (b.first == -1 || b.second == -1)
	{
		throw std::runtime_error("End position 'B' not found");
	}

	RoadMap dist;

	for (size_t y = 0; y < labyrinth.size(); y++)
	{
		std::vector<int> row;
		for (size_t x = 0; x < labyrinth[y].size(); x++)
		{
			row.push_back(-1);
		}
		dist.push_back(row);
	}

	std::queue<std::pair<int, int>> queue;

	dist[a.first][a.second] = 0;
	queue.push(a);

	while (!queue.empty())
	{
		auto current = queue.front();
		queue.pop();

		if (current == b)
			break;

		for (const auto& dir : DIRECTIONS)
		{
			int newY = current.first + dir.first;
			int newX = current.second + dir.second;

			if (newY < 0 || newY >= static_cast<int>(labyrinth.size())
				|| newX < 0 || newX >= static_cast<int>(labyrinth[newY].size()))
			{
				continue;
			}

			if (labyrinth[newY][newX] != '#' && dist[newY][newX] == -1)
			{
				dist[newY][newX] = dist[current.first][current.second] + 1;
				queue.push({ newY, newX });
			}
		}
	}
	return dist;
}

std::vector<std::pair<int, int>> RestorePath(const RoadMap& dist, const Position& start, const Position& end)
{
	std::vector<std::pair<int, int>> path;
	Position current = end;
	int currentDist = dist[current.first][current.second];

	if (currentDist == -1)
	{
		throw std::runtime_error("No path exists between A and B");
	}

	std::vector<std::pair<int, int>> restoreDirections = std::vector<std::pair<int, int>>(DIRECTIONS.rbegin(), DIRECTIONS.rend());

	while (current != start)
	{
		bool found = false;
		for (const auto& dir : restoreDirections)
		{
			int newY = current.first + dir.first;
			int newX = current.second + dir.second;

			if (newY < 0 || newY >= static_cast<int>(dist.size())
				|| newX < 0 || newX >= static_cast<int>(dist[newY].size()))
			{
				continue;
			}

			if (dist[newY][newX] == currentDist - 1)
			{
				path.push_back(current);
				current = { newY, newX };
				currentDist--;
				found = true;
				break;
			}
		}
		if (!found)
		{
			throw std::runtime_error("Path restoration failed");
		}
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}

void ProcessLabyrinth(Labyrinth& labyrinth)
{

	RoadMap dist = WaveAlgorithm(labyrinth);
	Position a = GetCharPosition(labyrinth, 'A');
	Position b = GetCharPosition(labyrinth, 'B');

	if (dist[b.first][b.second] == -1)
	{
		std::cerr << "ERROR: " << "Can't find path" << std::endl;
		return;
	}

	std::vector<std::pair<int, int>> path = RestorePath(dist, a, b);
	for (const auto& pos : path)
	{
		if (labyrinth[pos.first][pos.second] == ' ')
		{
			labyrinth[pos.first][pos.second] = '.';
		}
	}
}

int main(int argc, char* argv[])
{
	int errorCode = 1;

	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		std::cout << HELP_TEXT << std::endl;
		return 0;
	}

	Labyrinth labyrinth;
	InputMode mode = InputMode::STDIN;

	if (argc == 3)
	{
		mode = InputMode::FILE;
		errorCode = 0;
		std::ifstream inputFile(argv[1]);
		if (!inputFile.is_open())
		{
			std::cerr << "ERROR: " << "Can't open input file" << std::endl;
			return errorCode;
		}
		try
		{
			labyrinth = ReadLabyrinth(inputFile);
		}
		catch (std::exception& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
			return errorCode;
		}
	}
	else if (argc == 1)
	{
		try
		{
			labyrinth = ReadLabyrinth(std::cin);
		}
		catch (std::exception& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
			return errorCode;
		}
	}
	else
	{
		std::cerr << "Invalid arguments. Use -h for help." << std::endl;
		return 1;
	}

	try
	{
		ProcessLabyrinth(labyrinth);

		if (mode == InputMode::FILE)
		{
			std::ofstream outputFile(argv[2]);
			if (!outputFile.is_open())
			{
				std::cerr << "ERROR: " << "Can't open output file" << std::endl;
				return errorCode;
			}
			PrintLabyrinth(labyrinth, outputFile);
		}
		else
		{
			PrintLabyrinth(labyrinth, std::cout);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return errorCode;
	}

	return 0;
}