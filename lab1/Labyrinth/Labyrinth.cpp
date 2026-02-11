#include "Labyrinth.hpp"
#include "WaveAlgorithm.hpp"

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