#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>

#include "Node.h"
#include "Edge.h"



Edge::Edge()
{
    First = nullptr;
    Second = nullptr;
    setPosition(0, 0);
}

Edge::Edge(Node* a, Node* b, int val)
{
    First = a;
    Second = b;
    weight = val;
}

Edge::~Edge()
{
    delete First;
    delete Second;
}

void Edge::draw(sf::RenderTarget* rt, bool flag, sf::Color color, sf::Color textcolor, double rad_)
{

    double dist = sqrt(pow(First->getX() - Second->getX(), 2) + pow(First->getY() - Second->getY(), 2));
    double katet = abs(First->getX() - Second->getX());
    double angle = acos(katet / dist);
    angle = 180.0 / M_PI * angle;

    Node* left = (First->getX() <= Second->getX() ? First : Second);
    Node* right = (First->getX() > Second->getX() ? First : Second);

    double r = 0.11 * rad_;

    if (!flag)
    {
        sf::RectangleShape line(sf::Vector2f(dist, 4.f));
        line.setOrigin(dist / 2.0, 2);



        line.rotate((left->getY() <= right->getY() ? angle : 360.0 - angle));

        line.setPosition((right->getX() + left->getX()) / 2.0, abs(right->getY() + left->getY()) / 2.0);
        line.setFillColor(color);

        rt->draw(line);

        // Drawing Arrow
        double rad = 0.027 * rad_;

        sf::CircleShape  triangle(rad, 3);
        triangle.setFillColor(color);
        triangle.setOrigin(rad, rad);
        triangle.rotate((First->getY() <= Second->getY() ? 180.0 : -180.0) + (left->getY() <= right->getY() ? angle : -angle) + (left == First ? 30.0 : -30.0));
        triangle.setPosition(Second->getX() + (First->getX() - Second->getX()) / (dist / r), Second->getY() + (First->getY() - Second->getY()) / (dist / r));
        rt->draw(triangle);
    }

    if (flag)
    {
        sf::Text text;
        sf::Font font;
        font.loadFromFile("IBMPlexSans-Medium.ttf");
        text.setFont(font);
        
        text.setString(std::to_string(weight));
    	
		if (std::to_string(weight).length() > 2)
            text.setCharacterSize(0.033 * rad_);
        else text.setCharacterSize(0.041 * rad_);
    	
        text.setFillColor(textcolor);

        sf::FloatRect rect = text.getLocalBounds();
        double width = rect.width;
        double height = rect.height;


        text.setOrigin(width / 2.0, height / 2.0);

        sf::CircleShape circle;
        double radius = (width > height ? width / 1.5 : height / 1.5) + 0.0055 * rad_;
        circle.setRadius(radius);
        //circle.setScale(width + 2, height + 2);
        circle.setOrigin(radius, radius);
        circle.setFillColor(sf::Color::White);
        circle.setPosition((right->getX() + left->getX()) / 2.0, abs(right->getY() + left->getY()) / 2.0);
        circle.setPosition(Second->getX() + (First->getX() - Second->getX()) / (dist / (r + radius + 10)), Second->getY() + (First->getY() - Second->getY()) / (dist / (r + radius + 10)));
        rt->draw(circle);


        text.setPosition(Second->getX() + (First->getX() - Second->getX()) / (dist / (r + radius + 10)), Second->getY() + (First->getY() - Second->getY()) / (dist / (r + radius + 10)) - height / 2.0);
        rt->draw(text);
    }

}

// Getters

Node* Edge::getFirst()
{
    return First;
}

Node* Edge::getSecond()
{
    return Second;
}

// Setters

void Edge::setFirst(Node* a)
{
    First = a;
}

void Edge::setSecond(Node* b)
{
    Second = b;
}