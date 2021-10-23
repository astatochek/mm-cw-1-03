#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>
#include "limits.h"

#include "Greedy.h"

void DecreaseTemperature(double InitialTemperature, double& T, int iter)
{
    T = InitialTemperature * 0.1 / iter;
}

double Probability(int dE, double T)
{
    // Непосредственно хитрая формулка
    double P = exp(-dE / T);
    return P;
}

bool IsTransition(double P)
{
    double val = (double)rand() / (RAND_MAX);
    if (val <= P) return true;
    return false;
}

std::vector <int> GetCandidate(std::vector <int> route)
{
	if (route.back() == route.front())
		route.erase(route.end() - 1);
	
    int i = rand() % (route.size() - 1);
    int j = rand() % (route.size() - 1);
    while (j == i)
        j = rand() % (route.size() - 1);

    int val = route[i];
    route[i] = route[j];
    route[j] = val;

    return route;
}

int CalculateDist(std::vector <int> seq, std::vector <std::vector<int>> M) // считает расстояние по заданному пути
{
    int dist = 0;
    for (int i = 0; i < seq.size() - 1; i++) dist += M[seq[i]][seq[i + 1]];
    dist += M[seq[seq.size() - 1]][0];
    return dist;
}

std::pair <std::vector <int>, int> simulated_annealing(std::vector <std::vector<int>> M)
{
    std::cout.clear(std::ios_base::badbit);
    std::pair <std::vector <int>, int> greedyBest = Greedy(M);
    std::cout.clear(std::ios_base::goodbit);
	
    std::vector <int> Initial = greedyBest.first;
    Initial.erase(Initial.end()-1);
	
    double InitialTemperature = 100, MinTemperature = 2; // константы, влияющие на то, когда завершаются итерации и на вероятность использования плохой транспозиции
    double T = InitialTemperature;
    for (int i = 1; i < M.size(); i++) Initial.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(std::begin(Initial), std::end(Initial), e);
    Initial.insert(Initial.begin(), 0);
    int CurrentDist = CalculateDist(Initial, M);


    for (int iteration = 0; iteration < 2000; iteration++) // количество итераций тоже можно регулировать
    {
        std::vector <int> candidate = GetCandidate(Initial);
        // Если выгодно, то заменяем Initial на candidate
        if (CalculateDist(candidate, M) < CurrentDist)
        {
            Initial = candidate;
            CurrentDist = CalculateDist(candidate, M);
        }
        // Если нет, то решаем, как быть, используя формулку, зависящую от итерации
        // Надо, чтобы не попасть нечаянно в фейковый локальный минимум
        else
        {
            if (IsTransition(Probability((CalculateDist(candidate, M) - CurrentDist), T)))
            {
                Initial = candidate;
                CurrentDist = CalculateDist(candidate, M);
            }
        }
        DecreaseTemperature(InitialTemperature, T, iteration);
    }
    std::pair <std::vector<int>, int> p;
    Initial.push_back(Initial[0]);
    p.first = Initial;
    p.second = CurrentDist;
    return p;
}