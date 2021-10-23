#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Edge.h"


class Graph
{

private:
    sf::Vector2f center;
    std::vector <Node*> Nodes;
    std::vector <Edge*> Edges;
    std::vector <std::vector <int>> Matrix;

public:

    Graph();
    Graph(std::vector <std::vector <int>> M, double x, double y, double R); // матрица смежности
    ~Graph();
    void draw(sf::RenderTarget* rt, sf::Color color, sf::Color textcolor = sf::Color::Black);

    // Getters
    double R;
    std::vector <Node*> getNodes();
    std::vector <Edge*> getEdges();
    std::vector <std::vector <int>> getMatrix();
    int getSize();
    sf::Vector2f getCenter();

    // Setters
    void setMatrix(std::vector <std::vector <int>> M);
};
