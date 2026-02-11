#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

const std::string HELP_TEXT = "Usage: labyrinth <input file> <output file>\n"
							  "If u don't provide an input file, the program will read from stdin and out into stdout.\n"
							  "Proggram works with labyrinths represented as a grid of characters. The program will find the shortest path from 'A' to 'B' and mark it with '.'.\n"
							  "Maximum labyrinth size is 100x100. Allowed characters in labyrinth are: 'A', 'B', '#'. \n";

using Labyrinth = std::vector<std::vector<char>>;
const int MAX_SIZE = 100;