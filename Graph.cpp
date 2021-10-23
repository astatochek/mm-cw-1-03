#define _USE_MATH_DEFINES
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <vector>
#include <math.h>
#include <string>

#include "Node.h"
#include "Edge.h"
#include "Graph.h"

sf::Vector2f getCirclePoint(double radius, double angle) {
    return {
        static_cast<float>(radius * std::sin(angle)),
        static_cast<float>(-1. * radius * std::cos(angle))
    };
}





Graph::~Graph()
{
    for (auto& node : Nodes)
    {
        delete node;
    }
    delete& Nodes;
    for (auto& edge : Edges)
    {
        delete edge;
    }
    delete& Edges;
}


Graph::Graph(std::vector<std::vector <int>> M, double x, double y, double R_)
{
    R = R_;
    center = sf::Vector2f(x, y);
    int n = M.size();
    Matrix = M;
    for (int i = 0; i < n; i++)
    {
        sf::Vector2f point = sf::Vector2f(x, y) + getCirclePoint(R, i * 2 * M_PI / n);
        Node* tmp = new Node(point.x, point.y, std::to_string(i + 1));
        Nodes.push_back(tmp);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (M[i][j] != 0)
            {
                Edge* tmp = new Edge(Nodes[i], Nodes[j], M[i][j]);
                Edges.push_back(tmp);
            }

        }
    }

}

// Getters 
std::vector<std::vector<int>> Graph::getMatrix()
{
    return Matrix;
}

std::vector <Node*> Graph::getNodes()
{
    return Nodes;
}

std::vector <Edge*> Graph::getEdges()
{
    return Edges;
}

sf::Vector2f Graph::getCenter()
{
    return center;
}



// Drawing

void Graph::draw(sf::RenderTarget* rt, sf::Color color, sf::Color textcolor)
{
    for (auto& edge : Edges)
        edge->draw(rt, false, color, textcolor, R);
    for (auto& edge : Edges)
        edge->draw(rt, true, color, textcolor, R);
    for (auto& node : Nodes)
        node->draw(rt, color, R);
}