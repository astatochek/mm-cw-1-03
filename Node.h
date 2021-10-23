#pragma once
#include <SFML/Graphics.hpp>

class Node : public sf::CircleShape
{
private:
    std::string name;
    sf::Vector2f center;

public:
    Node();
    Node(double a, double b, std::string n);
    Node(sf::Vector2f c, std::string n) : center(c), name(n) {}

    // Drawing A Node
    void draw(sf::RenderTarget* rt, sf::Color color, double rad);

    // Getters
    std::string getName();
    double getX();
    double getY();

    // Setters
    void setName(std::string n);
};

