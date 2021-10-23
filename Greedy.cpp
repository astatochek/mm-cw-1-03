#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <iostream>

#include "Graph.h"

int Dist(std::vector <int> path, std::vector <std::vector <int>> M)
{
	int res = 0;
	for (int i = 0; i < path.size() - 1; i++)
		res += M[path[i]][path[i + 1]];
	return res;
}

std::pair <std::vector <int>, int> Greedy(std::vector <std::vector <int> > M)
{
	std::cout << "\nGreedy Launched\n";
	int n = M.size();

	std::vector <std::vector <int> > results;
	std::vector <int> dists;
	for (int i = 0; i < n; i++)
	{
		
		std::vector <int> path;
		std::vector <int> rest;
		for (int j = 0; j < n; j++)
			if (j != i)
				rest.push_back(j);
		
		path.push_back(i);
		while (!rest.empty())
		{
			int min = M[path.back()][rest[0]];
			int index = rest[0];
			for (auto &elem : rest)
			{
				if (min > M[path.back()][elem])
				{
					min = M[path.back()][elem];
					index = elem;
				}
			}

			path.push_back(index);
			rest.erase(find(rest.begin(), rest.end(), index));
		}

		path.push_back(path[0]);

		results.push_back(path);
		dists.push_back(Dist(path, M));
	}

	std::vector <int> path = results[0];
	int min = dists[0];

	for (int i = 0; i < n; i++)
	{
		if (min > dists[i])
		{
			min = dists[i];
			path = results[i];
		}
	}

	std::cout << "\nGreedy Algorithm\n";
	for (auto& elem : path)
		std::cout << elem + 1 << " ";
	std::cout << "\n" << min << " =? " << Dist(path, M);

	
	return std::make_pair(path, min);
}