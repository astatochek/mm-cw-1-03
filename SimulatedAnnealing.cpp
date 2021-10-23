#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>
#include "limits.h"



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

std::vector <int> GetCandidate(std::vector <int> v)
{
    // рандомно выберем 2 индекса и поменяем элементы местами
    std::vector <int> temp;
    for (int i = 1; i < v.size(); i++) temp.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(std::begin(temp), std::end(temp), e);
    int first_index = temp[0];
    int second_index = first_index;
    while (second_index == first_index) // получаем втторой индекс, не равный первому
    {
        std::shuffle(std::begin(temp), std::end(temp), e);
        second_index = temp[0];
    }
    int first = v[first_index];
    v[first_index] = v[second_index];
    v[second_index] = first;
    return v;
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
    std::vector <int> Initial;
    double InitialTemperature = 10, MinTemperature = 0.00001; // константы, влияющие на то, когда завершаются итерации и на вероятность использования плохой транспозиции
    double T = InitialTemperature;
    for (int i = 1; i < M.size(); i++) Initial.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(std::begin(Initial), std::end(Initial), e);
    Initial.insert(Initial.begin(), 0);
    int CurrentDist = CalculateDist(Initial, M);


    for (int iteration = 0; iteration < M.size() * M.size() + 100; iteration++) // количество итераций тоже можно регулировать
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
    Initial.push_back(0);
    p.first = Initial;
    p.second = CurrentDist;
    return p;
}
