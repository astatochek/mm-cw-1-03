#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <string>



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


Node::Node()
{
    name = "";
    center = sf::Vector2f(0, 0);
    setPosition(0, 0);
}

Node::Node(double a, double b, std::string n)
{
    center = sf::Vector2f(a, b);
    setPosition(a, b);
    name = n;
}

// Getters

std::string Node::getName()
{
    return name;
}

double Node::getX()
{
    return center.x;
}

double Node::getY()
{
    return center.y;
}


// Setters

void Node::setName(std::string n)
{
    name = n;
}

// Drawing a Node

void Node::draw(sf::RenderTarget* rt, sf::Color color, double rad)
{
    double radius = 0.05 * rad;
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(getPosition().x, getPosition().y);
    circle.setFillColor(sf::Color(46, 46, 46));
    circle.setOutlineThickness(2.f);
    circle.setOutlineColor(sf::Color(46, 46, 46));
    rt->draw(circle);

    sf::Text text;
    sf::Font font;
    font.loadFromFile("IBMPlexSans-Medium.ttf");
    text.setFont(font);
    text.setCharacterSize(0.06 * rad);
    text.setString(name);
    text.setFillColor(sf::Color::White);

    sf::FloatRect rect = text.getLocalBounds();
    double width = rect.width;
    double height = rect.height;




    text.setOrigin(width / 2.0, height / 2.0);

    text.setPosition(getPosition().x, getPosition().y - height / 2.0);
    rt->draw(text);
};