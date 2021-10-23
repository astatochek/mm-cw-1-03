#pragma once
#include <utility>
#include <vector>

struct bee
{
	int status; // active/inactive/scout
	std::vector <int> Path;
	int pathLen;
};

class Hive
{
	int totalPopulation;
	std::vector <bee> Colony;

	int numActive;
	std::vector <int> activeBees;
	int numInactive;
	std::vector <int> inactiveBees;
	int numScouts;
	std::vector <int> scoutBees;


	int maxEntryNum;

	int numElite;
	int numPerspective;

	double Temperature;
	double alpha;

	std::pair <std::vector <int>, int> bestPath;

	std::vector <std::vector <int>> M;

public:

	Hive(int n, double t, std::vector <std::vector <int>> Matrix, std::pair <std::vector <int>, int> greedyBest); // n % 100 == 0


	// Getters

	double getTemperature();

	std::pair <std::vector <int>, int> getBest();

	std::vector <int> getActive();

	std::vector <int> getInactive();

	std::vector <int> getScouts();


	// Functions


	int f(std::vector <int> path);

	void UseScout(int i);

	void UseActive(int i, std::vector <int> route);

	void Dance(bee bee, int sum);

	std::vector <std::vector <int> > getLocalPaths();

	void ProcessIteration();

	std::vector <int> selectParents();

	bee Merge(bee firstParent, bee secondParent);

	std::vector <bee> Hybridization();

	void LocalOptimisation();
};

std::pair <std::vector<int>, int> hybridBeeColonyAlg(std::vector <std::vector<int>> M);
