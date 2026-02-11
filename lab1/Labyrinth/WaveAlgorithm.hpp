#pragma once

#include "Labyrinth.hpp"
#include <vector>
#include <queue>

using RoadMap = std::vector<std::vector<int>>;
using Position = std::pair<int, int>;
const std::vector<std::pair<int, int>> DIRECTIONS = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

Position GetCharPosition(const Labyrinth& labyrinth, const char& ch);

std::vector<std::pair<int, int>> RestorePath(const RoadMap& dist, const Position& start, const Position& end);

RoadMap WaveAlgorithm(const Labyrinth& labyrinth);
