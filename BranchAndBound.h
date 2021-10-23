#pragma once

#include <SFML/Graphics.hpp>

#include "Graph.h"

std::pair <std::vector <int>, int> BranchAndBound(Graph* graph);
