#include "WaveAlgorithm.hpp"

Position GetCharPosition(const Labyrinth& labyrinth, const char& ch)
{
	for (size_t y = 0; y < labyrinth.size(); y++)
	{
		for (size_t x = 0; x < labyrinth[y].size(); x++)
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
