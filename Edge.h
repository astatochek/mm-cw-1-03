#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"

class Edge : public sf::RectangleShape
{
private:

    Node* First;
    Node* Second;
    int weight;

public:

    Edge();
    Edge(Node* a, Node* b, int val);
    ~Edge();

    // Drawing An Edge
    void draw(sf::RenderTarget* rt, bool flag, sf::Color color, sf::Color textcolor, double rad_);

    // Getters
    Node* getFirst();
    Node* getSecond();
    int getWeight();

    // Setters
    void setFirst(Node* a);
    void setSecond(Node* b);
    void setWeight();


};
