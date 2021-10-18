#include <utility>
#include <vector>
#include <cmath>
#include <time.h>
#include <iostream>
#include <random>
#include <limits.h>
#include <algorithm>



struct ant // ��������� ������� ��� ����, ����� ��� ������� �������
{
	std::vector <bool> TabooList; // ���������� ������� � �������� ������� �����
	std::vector <int> trace = {}; // ����
	int Location;
	int StartLocation;
	ant(int CurrentLocation, int n)
	{
		std::vector <bool> temp(n, false);
		TabooList = temp;
		Location = CurrentLocation;
		StartLocation = CurrentLocation;
		TabooList[CurrentLocation] = true;
		trace.push_back(Location);
	}

};

struct edge // ��������� ��� �����
{
	int dist;
	double PheromoneLevel;
	double Evaporation;
	double BonusPheromones;
	edge(double CurrentPheromoneLevel, double CurrentEvapoartion, int Mdist)
	{
		dist = Mdist;
		PheromoneLevel = CurrentPheromoneLevel;
		Evaporation = CurrentEvapoartion;
		BonusPheromones = 0;
	}
	void UpdatePheromones()
	{
		PheromoneLevel = PheromoneLevel * Evaporation + BonusPheromones;
		BonusPheromones = 0;
	}
};


void Refresh(std::vector <ant>& Colony, int n) // ����������� �������� �� ��������� �������
{
	std::vector <ant> temp;
	for (int i = 0; i < n; i++)
		temp.push_back(ant(i, n));
	Colony = temp;
}


void NextPosition(ant& Ant, int alpha, int beta, std::vector <std::vector <edge>>& M) // ����������, ���� ���� ������� ������
{
	int location = Ant.Location;
	int n = M.size();
	long double sum = 0;
	for (int i = 0; i < n; i++)
	{
		if (!Ant.TabooList[i] && i != location)
			sum += pow(M[location][i].PheromoneLevel, alpha) * pow(1.0 / long double(M[location][i].dist), beta);	
	}
	std::vector <std::pair<int, long double>> Vector; // ������ ��� ���������� ����������� ������
	long double addition = 0;
	for (int i = 0; i < n; i++) // ������� ������������ ��� ������� �� ��������� ����� ����������
	{
		if (!Ant.TabooList[i] && i != location)
		{
			std::pair <int, double> tmp;
			tmp.first = i;
			addition += pow(M[location][i].PheromoneLevel, alpha) * pow(1.0 / long double(M[location][i].dist), beta) / sum;
			tmp.second = addition;
			Vector.push_back(tmp);
		}
	}
	srand(time(0));
	double chance = double(rand() % 1000) / 1000.0;
	for (int i = 0; i < Vector.size(); i++) // ����� ������ �� ����� ���������� �������
		if (chance <= Vector[i].second || Vector.size() == 1)
		{
			Ant.TabooList[Vector[i].first] = true;
			M[location][Vector[i].first].BonusPheromones += 1.0 / long double(M[location][Vector[i].first].dist);
			Ant.Location = Vector[i].first;
			Ant.trace.push_back(Vector[i].first);
			break;
		}
	
	

}

int Dist(std::vector <int> path, std::vector <std::vector <edge>> M)
{
	int res = 0;
	for (int i = 0; i < path.size() - 1; i++)
		res += M[path[i]][path[i + 1]].dist;
	return res;
}


std::pair <std::vector<int>, int> ant_colony(std::vector <std::vector<int>> Mprev)
{

	std::cout << "\nAnt Colony Launched\n";
	std::vector <std::vector <edge>> M;
	std::vector <edge> temp;
	int n = Mprev.size();

	double CurrentPheromoneLevel = 100; // ���������, ������������ ��������� ������� ��������� �� ������ �� �����
	double CurrentEvaporation = 0.9; // ���������, ������������ ������� ���������� ����� ��������� ��������� ��� ������� ����� � ����� �������� �����
	int alpha = 2; // �������������� ������ ���������
	int beta = 3; // �������������� ����� ����
	// ����� � ���� ������������ � ������� ���������� ����������� ��� ������ ���������� ����� �� ���� �������
	std::pair <std::vector <int>, int> bestRoute;
	for (int i = 0; i < n; i++) // ���������� ���������� ������� ���������
	{
		temp = {};
		for (int j = 0; j < n; j++)
			temp.push_back(edge(CurrentPheromoneLevel, CurrentEvaporation, Mprev[i][j]));
		M.push_back(temp);
	}
	std::vector <ant> Colony;
	for (int i = 0; i < n; i++)
		Colony.push_back(ant(i, n));
	
	
	int Iterations = n * n + 1000; // ���������� �������� �����
	while (Iterations--)
	{
		std::cout << Iterations << "\n";
		
		Refresh(Colony, n);
		
		for (auto& Ant : Colony) // ��� ������� �������
		{
			for (int j = 0; j < n - 1; j++) // ����� ��������� �������
			{
				NextPosition(Ant, alpha, beta, M);
			}
			Ant.TabooList[Ant.StartLocation] = false;
			NextPosition(Ant, alpha, beta, M); // ���������� �������� ������� �������
			std::vector <int> route = Ant.trace;

			if (bestRoute.first.empty() && route.size() == M.size() + 1)
				bestRoute = std::make_pair(route, Dist(route, M));
			
			if (Dist(route, M) < bestRoute.second && route.size() == M.size() + 1)
				bestRoute = std::make_pair(route, Dist(route, M));
		}
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				M[i][j].UpdatePheromones();
	}



	std::cout << "\nAnt Colony\n";
	for (auto& elem : bestRoute.first)
		std::cout << elem + 1 << " ";
	std::cout << "\n" << bestRoute.second << " =? " << Dist(bestRoute.first, M);



	return(bestRoute);
}