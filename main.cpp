#define _USE_MATH_DEFINES
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <windows.h>
#include <vector>
#include <string>
#include <ctime>
#include <random>


#include "Graph.h"


#include "BranchAndBound.h"
#include "Greedy.h"
#include "antColony.h"
#include "HBCA.h"
#include "SimulatedAnnealing.h"


void draw_text(sf::RenderTarget* rt, std::string str, double x, double y, bool position)
{
    sf::Text text;
    sf::Font font;
    font.loadFromFile("IBMPlexSans-Medium.ttf");
    text.setFont(font);
    text.setCharacterSize(25);
    text.setString(str);
    text.setFillColor(sf::Color(46, 46, 46));
	if (position)
    {
        sf::FloatRect rect = text.getLocalBounds();
        double width = rect.width;
        double height = rect.height;
        text.setOrigin(width / 2.0, height / 2.0);
    }
    text.setPosition(x, y);
    rt->draw(text);
}

void draw_path(sf::RenderTarget* rt, Graph* graph, std::vector <int> res, int result)
{
    graph->draw(rt, sf::Color(234, 234, 234), sf::Color(234, 234, 234));


    for (int i = 0; i < res.size() - 1; i++)
    {
        int first = res[i];
        int second = res[i + 1];




        for (auto& edge : graph->getEdges())
        {
            if (edge->getFirst()->getName() == std::to_string(first + 1) && edge->getSecond()->getName() == std::to_string(second + 1))
            {
                edge->draw(rt, false, sf::Color(27, 134, 217), sf::Color::Black, graph->R);
            }
        }


    }


    for (int i = 0; i < res.size() - 1; i++)
    {
        int first = res[i];
        int second = res[i + 1];




        for (auto& edge : graph->getEdges())
        {
            if (edge->getFirst()->getName() == std::to_string(first + 1) && edge->getSecond()->getName() == std::to_string(second + 1))
            {
                edge->draw(rt, true, sf::Color(27, 134, 217), sf::Color::Black, graph->R);
            }
        }


    }



    for (auto& node : graph->getNodes())
    {
        node->draw(rt, sf::Color(27, 134, 217), graph->R);
    }
}

std::vector <std::vector <int> > makeMatrix(int n, bool complete)
{
    std::mt19937 engine;
    engine.seed(std::time(nullptr));
    std::random_device device;
    engine.seed(device());

    std::vector <std::vector <int> > M(n, std::vector <int>(n, 0));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
            int elem = engine() % 1000;
			if (complete && elem == 0)
			{
				while (elem == 0)
				{
                    elem = engine() % 1000;
				}
			}
			if (!complete)
			{
                int flag = engine();
                if (flag % 2 == 0)
                    elem = 0;
			}

            M[i][j] = elem;
            M[j][i] = elem;
		}

		if (!complete)
		{
			for (int i = 0; i < n; i++)
			{
                bool flag = true;
				for (int j = 0; j < n; j++)
					if (M[i][j] != 0)
                        flag = false;
				if (flag)
				{
                    int j = engine() % n;
                    while (j == i)
                        j = engine() % n;
                    int elem = engine() % 100 + 1;
                    M[i][j] = elem;
                    M[j][i] = elem;
				}
			}
		}
	}

	
    return M;
}


int main()
{
    const unsigned width = 1800;
    const unsigned height = 1000;
    sf::RenderWindow window(sf::VideoMode(width, height), "Window");

    sf::RenderTexture rt;

    double W_width = window.getSize().x;
    double W_height = window.getSize().y;

    rt.create(width, height);

    int n = 1;
    int k = 30;



    std::vector <std::vector <int> > matrix;

    std::vector <std::vector <std::vector <int> >> Matrices;

    std::vector < std::pair <std::vector <int>, int> > branchVector(n);
    std::vector < std::pair <std::vector <int>, int> > greedyVector(n);
    std::vector < std::pair <std::vector <int>, int> > antVector(n);
    std::vector < std::pair <std::vector <int>, int> > beeVector(n);
    std::vector < std::pair <std::vector <int>, int> > simVector(n);

	for (int i = 0; i < n; i++)
	{
        matrix = makeMatrix(k + i, true);
        Matrices.push_back(matrix);
	}

    int index = 0;


    if (true)
    {
    	if (k < 21)
    	{
            Graph* branchGraph = new Graph(Matrices[index], W_width / 4, W_height / 2, 360);

            if (branchVector[index].first.empty())
            {
                branchVector[index] = BranchAndBound(branchGraph);
            }

            draw_text(&rt, "Branch and Bound: " + std::to_string(branchVector[index].second), W_width / 4, 40, true);

            draw_path(&rt, branchGraph, branchVector[index].first, branchVector[index].second);
    	}
        


        // Жадный

        Graph* greedyGraph = new Graph(Matrices[index], W_width * 7 / 8, W_height * 3 / 4, 150);

        if (greedyVector[index].first.empty())
        {
            greedyVector[index] = Greedy(greedyGraph->getMatrix());
        }

        draw_text(&rt, "Greedy Algorithm: " + std::to_string(greedyVector[index].second), W_width * 7 / 8, W_height / 2, true);

        draw_path(&rt, greedyGraph, greedyVector[index].first, greedyVector[index].second);


        // Муравьи

        Graph* antGraph = new Graph(Matrices[index], W_width * 5 / 8, W_height * 3 / 4, 150);

        if (antVector[index].first.empty())
        {
            antVector[index] = ant_colony(antGraph->getMatrix());
        }

        draw_text(&rt, "Ant Colony Algorithm: " + std::to_string(antVector[index].second), W_width * 5 / 8, W_height / 2, true);

        draw_path(&rt, antGraph, antVector[index].first, antVector[index].second);


        // Пчелы


        Graph* beeGraph = new Graph(Matrices[index], W_width * 5 / 8, W_height / 4, 150);

        if (beeVector[index].first.empty())
        {
            beeVector[index] = hybridBeeColonyAlg(beeGraph->getMatrix());
        }

        draw_text(&rt, "Hybrid Bee Algorithm: " + std::to_string(beeVector[index].second), W_width * 5 / 8, 20, true);

        draw_path(&rt, beeGraph, beeVector[index].first, beeVector[index].second);

        // Отжиг

        Graph* simGraph = new Graph(Matrices[index], W_width * 7 / 8, W_height / 4, 150);

        if (simVector[index].first.empty())
        {
            simVector[index] = simulated_annealing(simGraph->getMatrix());
        }

        draw_text(&rt, "Simulated Annealing: " + std::to_string(simVector[index].second), W_width * 7 / 8, 20, true);

        draw_path(&rt, simGraph, simVector[index].first, simVector[index].second);
    }

    

    

    
    //bool complete = true;


    rt.display();
    sf::Sprite sprite(rt.getTexture());
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        sf::Event event;
    	
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Num4))
            {
                rt.clear(sf::Color::White);
            	
                Graph* branchGraph = new Graph(Matrices[index], W_width / 4, W_height / 2, 360);

                if (branchVector[index].first.empty())
                {
                    branchVector[index] = BranchAndBound(branchGraph);
                }

                draw_text(&rt, "Branch and Bound: " + std::to_string(branchVector[index].second), W_width / 4, 40, true);

                draw_path(&rt, branchGraph, branchVector[index].first, branchVector[index].second);

                if (true)
                {
                    // Жадный

                    Graph* greedyGraph = new Graph(Matrices[index], W_width * 7 / 8, W_height * 3 / 4, 150);

                	if (event.key.code == sf::Keyboard::Num4)
						greedyVector[index] = Greedy(greedyGraph->getMatrix());

                    draw_text(&rt, "Greedy Algorithm: " + std::to_string(greedyVector[index].second), W_width * 7 / 8, W_height / 2, true);

                    draw_path(&rt, greedyGraph, greedyVector[index].first, greedyVector[index].second);


                    // Муравьи

                    Graph* antGraph = new Graph(Matrices[index], W_width * 5 / 8, W_height * 3 / 4, 150);

                	if (event.key.code == sf::Keyboard::Num3)
						antVector[index] = ant_colony(antGraph->getMatrix());

                    draw_text(&rt, "Ant Colony Algorithm: " + std::to_string(antVector[index].second), W_width * 5 / 8, W_height / 2, true);

                    draw_path(&rt, antGraph, antVector[index].first, antVector[index].second);


                    // Пчелы


                    Graph* beeGraph = new Graph(Matrices[index], W_width * 5 / 8, W_height / 4, 150);

                    if (event.key.code == sf::Keyboard::Num1)
						beeVector[index] = hybridBeeColonyAlg(beeGraph->getMatrix());

                    draw_text(&rt, "Hybrid Bee Algorithm: " + std::to_string(beeVector[index].second), W_width * 5 / 8, 20, true);

                    draw_path(&rt, beeGraph, beeVector[index].first, beeVector[index].second);

                    // Отжиг

                    Graph* simGraph = new Graph(Matrices[index], W_width * 7 / 8, W_height / 4, 150);

                	if (event.key.code == sf::Keyboard::Num2)
						simVector[index] = simulated_annealing(simGraph->getMatrix());

                    draw_text(&rt, "Simulated Annealing: " + std::to_string(simVector[index].second), W_width * 7 / 8, 20, true);

                    draw_path(&rt, simGraph, simVector[index].first, simVector[index].second);
                }

            	
            }
        	
            
        	

            if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down))
            {
                rt.clear(sf::Color::White);

                if (event.key.code == sf::Keyboard::Up)
                    index = (index + 1) % n;
                else if (event.key.code == sf::Keyboard::Down)
                    index = (index - 1 + n) % n;



                if (true)
                {
                	if (k < 21)
                	{
                        Graph* branchGraph = new Graph(Matrices[index], W_width / 4, W_height / 2, 360);

                        if (branchVector[index].first.empty())
                        {
                            branchVector[index] = BranchAndBound(branchGraph);
                        }

                        draw_text(&rt, "Branch and Bound: " + std::to_string(branchVector[index].second), W_width / 4, 40, true);

                        draw_path(&rt, branchGraph, branchVector[index].first, branchVector[index].second);
                	}
                    


                    // Жадный

                    Graph* greedyGraph = new Graph(Matrices[index], W_width * 7 / 8, W_height * 3 / 4, 150);

                    if (greedyVector[index].first.empty())
                    {
                        greedyVector[index] = Greedy(greedyGraph->getMatrix());
                    }

                    draw_text(&rt, "Greedy Algorithm: " + std::to_string(greedyVector[index].second), W_width * 7 / 8, W_height / 2, true);

                    draw_path(&rt, greedyGraph, greedyVector[index].first, greedyVector[index].second);


                    // Муравьи

                    Graph* antGraph = new Graph(Matrices[index], W_width * 5 / 8, W_height * 3 / 4, 150);

                    if (antVector[index].first.empty())
                    {
                        antVector[index] = ant_colony(antGraph->getMatrix());
                    }

                    draw_text(&rt, "Ant Colony Algorithm: " + std::to_string(antVector[index].second), W_width * 5 / 8, W_height / 2, true);

                    draw_path(&rt, antGraph, antVector[index].first, antVector[index].second);


                    // Пчелы


                    Graph* beeGraph = new Graph(Matrices[index], W_width * 5 / 8, W_height / 4, 150);

                    if (beeVector[index].first.empty())
                    {
                        beeVector[index] = hybridBeeColonyAlg(beeGraph->getMatrix());
                    }

                    draw_text(&rt, "Hybrid Bee Algorithm: " + std::to_string(beeVector[index].second), W_width * 5 / 8, 20, true);

                    draw_path(&rt, beeGraph, beeVector[index].first, beeVector[index].second);

                    // Отжиг

                    Graph* simGraph = new Graph(Matrices[index], W_width * 7 / 8, W_height / 4, 150);

                    if (simVector[index].first.empty())
                    {
                        simVector[index] = simulated_annealing(simGraph->getMatrix());
                    }

                    draw_text(&rt, "Simulated Annealing: " + std::to_string(simVector[index].second), W_width * 7 / 8, 20, true);

                    draw_path(&rt, simGraph, simVector[index].first, simVector[index].second);
                }

            }

        	
        }
        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }


    return 0;
}