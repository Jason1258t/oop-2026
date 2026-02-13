#include "Labyrinth.hpp"
#include "WaveAlgorithm.hpp"

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
		throw std::runtime_error("Can't find path");
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
	int modeBasedErrorCode;
	std::string inputFile;
	std::string outputFile;
};

ProgrammArgs ParseProgrammArgs(int argc, char* argv[])
{
	if (argc == 2 && std::string(argv[1]) == "-h")
	{
		return { ProgrammMode::HELP };
	}

	if (argc == 3)
	{
		return { ProgrammMode::HELP, 0, argv[1], argv[2] };
	}
	if (argc == 1)
	{
		return { ProgrammMode::STDIN, 1 };
	}
	return { ProgrammMode::INVALID };
}

void ProcessFileMode(const ProgrammArgs& args)
{
	std::ifstream inputFile(args.inputFile);
	if (!inputFile.is_open())
	{
		throw std::runtime_error("Can't open file " + args.inputFile);
	}

	Labyrinth labyrinth = ReadLabyrinth(inputFile);
	ProcessLabyrinth(labyrinth);
	std::ofstream outputFile(args.outputFile);
	if (!outputFile.is_open())
	{
		throw std::runtime_error("Can't open file " + args.outputFile);
	}
	PrintLabyrinth(labyrinth, outputFile);
}

void ProcessUserInputMode(const ProgrammArgs& args)
{
	Labyrinth labyrinth = ReadLabyrinth(std::cin);
	ProcessLabyrinth(labyrinth);
	PrintLabyrinth(labyrinth, std::cout);
}

int main(int argc, char* argv[])
{
	auto args = ParseProgrammArgs(argc, argv);

	if (args.mode == ProgrammMode::HELP)
	{
		std::cout << HELP_TEXT << std::endl;
		return 0;
	}

	if (args.mode == ProgrammMode::INVALID)
	{
		std::cerr << "Invalid arguments. Use -h for help." << std::endl;
		return 1;
	}

	try
	{
		if (args.mode == ProgrammMode::FILE)
		{
			ProcessFileMode(args);
		}
		else if (args.mode == ProgrammMode::STDIN)
		{
			ProcessUserInputMode(args);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return args.modeBasedErrorCode;
	}

	return 0;
}