/*
#define inactive 1
#define active 2
#define scout 3
*/

#include <utility>
#include <vector>
#include <cstdlib>
#include <limits.h>
#include <cmath>
#include <time.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

#include "HBCA.h"
#include "Greedy.h"



std::vector <int> Transpose(std::vector <int> route)
{
	route.erase(route.end() - 1);
	int i = rand() % (route.size() - 1);
	int j = rand() % (route.size() - 1);
	while (j == i)
		j = rand() % (route.size() - 1);

	int val = route[i];
	route[i] = route[j];
	route[j] = val;

	route.push_back(route[0]);
	
	return route;
}


Hive::Hive(int n, double t, std::vector <std::vector <int>> Matrix, std::pair <std::vector <int>, int> greedyBest) // n % 100 == 0
{
	totalPopulation = n;
	numActive = (n * 3) / 4;
	numInactive = n / 10;
	numScouts = (n * 3) / 20;

	M = Matrix;
	Temperature = t;
	alpha = 0.999;

	numElite = 2;
	numPerspective = 3;


	bestPath = greedyBest;

	for (int i = 0; i < totalPopulation; i++)
	{
		bee tempBee;
		tempBee.pathLen = INT_MAX;

		if (i < numActive)
		{
			tempBee.status = 2;
			activeBees.push_back(i);
		}



		else if (i < numActive + numInactive)
		{
			tempBee.status = 1;
			inactiveBees.push_back(i);
		}


		else
		{
			tempBee.status = 3;
			scoutBees.push_back(i);
		}


		Colony.push_back(tempBee);
	}

}

double Hive::getTemperature()
{
	return Temperature;
}

std::pair <std::vector <int>, int> Hive::getBest()
{
	return bestPath;
}

std::vector <int> Hive::getActive()
{
	return activeBees;
}

std::vector <int> Hive::getInactive()
{
	return inactiveBees;
}

std::vector <int> Hive::getScouts()
{
	return scoutBees;
}

int Hive::f(std::vector <int> path) // фитнес функция
{
	int res = 0;
	for (int i = 0; i < path.size() - 1; i++)
		res += M[path[i]][path[i + 1]];
	return res;
}

void Hive::UseScout(int i) // используем пчелу скаута
{
	std::vector <int> path;
	for (int i = 0; i < M.size(); i++)
		path.push_back(i);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine e(seed);
	std::shuffle(std::begin(path), std::end(path), e);
	path.push_back(path[0]);

	int fit = f(path);

	Colony[i].Path = path;
	Colony[i].pathLen = fit;


	if (Colony[i].pathLen < bestPath.second)
	{
		bestPath = std::make_pair(Colony[i].Path, Colony[i].pathLen);
		std::cout << bestPath.second << "\n";
	}


}

void Hive::UseActive(int i, std::vector <int> route) // используем активную пчелу
{

	std::vector <int> semiPath = Transpose(Transpose(route));
	int semiPathLen = f(semiPath);

	if (Colony[i].pathLen > semiPathLen) // если новый путь лучше предыдущего лучшего
	{
		Colony[i].Path = semiPath;
		Colony[i].pathLen = semiPathLen;

		if (semiPathLen < bestPath.second)
		{
			bestPath = std::make_pair(Colony[i].Path, Colony[i].pathLen);
			std::cout << bestPath.second << "\n";
		}
	}
	else
	{
		double chance = (rand() * 10000) / 10000.0;

		if (exp(-(semiPathLen - Colony[i].pathLen) / Temperature) > chance)
		{
			Colony[i].Path = semiPath;
			Colony[i].pathLen = semiPathLen;
			Temperature *= alpha;
		}
	}

}

void Hive::Dance(bee Bee, int sum) // вербовка
{

	double possibility = 0.6;

	std::vector <bee> newActive;
	
	for (auto& inactiveIndex : inactiveBees)
	{
		double chance = (rand() % 10000) / 10000.0;
		if (chance > possibility && Bee.pathLen <= 1.5 * (double) sum / (numActive + numScouts))
		{
			Colony[inactiveIndex].Path = Bee.Path;
			Colony[inactiveIndex].pathLen = Bee.pathLen;
			newActive.push_back(Bee);
			newActive.push_back(Colony[inactiveIndex]);
		}
	}

	for (int i = 1; i < newActive.size(); i++)
		for (int j = i; j > 0 && newActive[j - 1].pathLen > newActive[j].pathLen; j--)
			std::swap(newActive[j - 1], newActive[j]);

	
	int i = 0;
	for (auto& activeIndex : activeBees)
	{
		if (i >= newActive.size())
			break;
		else
		{
			Colony[activeIndex].Path = newActive[i].Path;
			Colony[activeIndex].pathLen = newActive[i].pathLen;
			i++;
		}
	}

	

}

std::vector <std::vector <int> > Hive::getLocalPaths()
{

	for (int i = 1; i < numScouts; i++)
		for (int j = i; j > 0 && Colony[scoutBees[j - 1]].pathLen < Colony[scoutBees[j]].pathLen; j--)
			std::swap(scoutBees[j - 1], scoutBees[j]);

	std::vector <std::vector <int> > res;
	for (int i = 0; i < numElite + numPerspective; i++)
		res.push_back(Colony[scoutBees[i]].Path);

	return res;
}

void Hive::ProcessIteration()
{

	for (auto& scoutIndex : scoutBees)
		UseScout(scoutIndex);

	std::vector <std::vector <int> > paths = getLocalPaths();

	for (int ix = 0; ix < numActive; ix++) // предполагается, что элитных 2, а перспективных 3.
	{
		if (ix < numActive / 3)
			UseActive(activeBees[ix], paths[0]);
		else if (ix < numActive * 2 / 3)
			UseActive(activeBees[ix], paths[1]);
		else if (ix < numActive * 7 / 9)
			UseActive(activeBees[ix], paths[2]);
		else if (ix < numActive * 8 / 9)
			UseActive(activeBees[ix], paths[3]);
		else
			UseActive(activeBees[ix], paths[4]);
	}


}

std::vector <int> Hive::selectParents() // выбираем пчел для скрещивания
{

	std::vector <int> Parents;

	for (auto& scoutIndex : scoutBees)
	{
		double chance = (rand() % 10000) / 10000.0;
		if (exp(-(Colony[scoutIndex].pathLen - bestPath.second) / Temperature) > chance)
		{
			Parents.push_back(scoutIndex);
		}
	}

	for (auto& activeIndex : activeBees)
	{
		double chance = (rand() % 10000) / 10000.0;
		if (exp(-(Colony[activeIndex].pathLen - bestPath.second) / Temperature) > chance)
		{
			Parents.push_back(activeIndex);
		}
	}

	Temperature *= alpha;

	return Parents;
}

bee Hive::Merge(bee firstParent, bee secondParent) // Скрещивание двух пчел
{

	std::vector <int> firstPath = firstParent.Path;
	firstPath.erase(firstPath.end() - 1);
	std::vector <int> secondPath = secondParent.Path;
	secondPath.erase(secondPath.end() - 1);

	while (firstPath[0] != 0)
	{
		firstPath.push_back(firstPath[0]);
		firstPath.erase(firstPath.begin());
	}
	while (secondPath[0] != 0)
	{
		secondPath.push_back(secondPath[0]);
		secondPath.erase(secondPath.begin());
	}

	std::vector <int> res;
	std::vector <bool> mask(firstPath.size(), false);

	for (auto& elemF : firstPath)
		for (auto& elemS : secondPath)
		{
			if (mask[elemF] && !mask[elemS])
			{
				res.push_back(elemS);
				mask[elemS] = true;
				break;
			}
			else if (mask[elemS] && !mask[elemF])
			{
				res.push_back(elemF);
				mask[elemF] = true;
			}
			else if (!mask[elemF] && !mask[elemS])
			{
				double chance = (rand() % 10000) / 10000.0;
				if (chance < 0.5)
				{
					res.push_back(elemS);
					mask[elemS] = true;
				}
				else
				{
					res.push_back(elemF);
					mask[elemF] = true;
				}

				break;
			}
		}

	res.push_back(res[0]);
	bee bee;
	bee.Path = res;
	bee.pathLen = f(res);

	if (bee.pathLen < bestPath.second)
	{
		bestPath.first = bee.Path;
		bestPath.second = bee.pathLen;
		std::cout << bestPath.second << "\n";
	}

	return bee;
}

std::vector <bee> Hive::Hybridization()
{

	std::vector <int> Parents = selectParents();

	int Iter = Parents.size();
	std::vector <bee> Children;
	int sum = 0;
	double addition = 0;

	for (auto& Index : Parents)
		sum += 1 / (double)Colony[Index].pathLen;


	std::vector <std::pair <int, double> > available; // {index, value}

	for (auto& Index : Parents)
	{
		addition += (1 / (double)Colony[Index].pathLen) / sum;
		available.push_back(std::make_pair(Index, addition));
	}


	for (int l = 0; l < Iter; l++)
	{
		int i = 0, j = 0;
		double chance = (rand() % 10000) / 10000.0;
		for (auto& pairIndexValue : available)
			if (pairIndexValue.second >= chance)
			{
				i = pairIndexValue.first;
				break;
			}

		chance = 1 - (rand() % 10000) / 10000.0;
		for (auto& pairIndexValue : available)
			if (pairIndexValue.second >= chance)
			{
				j = pairIndexValue.first;
				break;
			}


		Children.push_back(Merge(Colony[i], Colony[j]));
	}
	bee bestBee;
	bestBee.Path = bestPath.first;
	bestBee.pathLen = bestPath.second;

	for (int l = 0; l < Iter; l++) // скрещиваем лучшую пчелу со всеми доступными
	{
		Children.push_back(Merge(bestBee, Colony[Parents[l]]));
	}

	for (auto& index : Parents)
		Children.push_back(Colony[index]);

	Children.push_back(bestBee);

	for (int i = 1; i < Children.size(); i++)
		for (int j = i; j > 0 && Children[j - 1].pathLen < Children[j].pathLen; j--)
			std::swap(Children[j - 1], Children[j]);

	return Children;
}

void Hive::LocalOptimisation()
{

	double LocalTemperature = 5000;
	double localAlpha = 0.95;
	double minLocalTemperature = 20;

	bee bee;
	bee.Path = bestPath.first;
	bee.pathLen = bestPath.second;

	while (LocalTemperature >= minLocalTemperature)
	{
		std::vector <int> newPath = Transpose(Transpose(bee.Path));
		int fit = f(newPath);

		if (fit < bestPath.second)
		{
			bestPath.first = newPath;
			bestPath.second = fit;
			std::cout << fit << "\n";
		}


		if (fit < bee.pathLen)
		{
			bee.Path = newPath;
			bee.pathLen = fit;
		}
		else
		{
			double chance = (rand() % 10000) / 10000.0;

			LocalTemperature *= localAlpha;

			if (exp(-(fit - bee.pathLen) / LocalTemperature) > chance)
			{
				bee.Path = newPath;
				bee.pathLen = fit;
			}
		}

	}

}


std::pair <std::vector<int>, int> hybridBeeColonyAlg(std::vector <std::vector<int>> M)
{
	std::cout << "\nBees Launched\n";
	
	int n = 100;
	double t = 5000;
	double temperatureLimit = 2;
	int Iterations = 1000;

	std::cout.clear(std::ios_base::badbit);
	std::pair <std::vector <int>, int> greedyBest = Greedy(M);
	std::cout.clear(std::ios_base::goodbit);
	
	Hive Hive(n, t, M, std::make_pair(std::vector <int> {0}, INT_MAX));
	while (Iterations-- && Hive.getTemperature() >= temperatureLimit)
	{
		std::cout << Iterations << " : " << Hive.getBest().second << "\n";
		Hive.ProcessIteration(); // скауты и активные выбрали пути
		std::vector <bee> children = Hive.Hybridization();
		int sum = 0;
		
		for (auto& bee : children)
			sum += bee.pathLen;
		
		for (auto& bee : children)
			Hive.Dance(bee, sum);

		Hive.LocalOptimisation();


	}

	std::cout << "\nBee Colony\n";
	for (auto& elem : Hive.getBest().first)
		std::cout << elem + 1 << " ";
	std::cout << "\n" << Hive.getBest().second << "\n";

	return Hive.getBest();
	
}