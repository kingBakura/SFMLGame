//  includes
#include "SFML-2.6.0\include\SFML\Graphics.hpp"

//  globals
sf::Vector2f viewSize(1024, 768);
sf::VideoMode videoMode(viewSize.x, viewSize.y);
sf::RenderWindow window(videoMode, "SFML", sf::Style::Default);

//  functions
int main(void)
{
    sf::RectangleShape rect(sf::Vector2f(500.0f, 300.0f));

    rect.setFillColor(sf::Color::Yellow);
    rect.setPosition(viewSize.x / 2, viewSize.y / 2);
    rect.setOrigin(sf::Vector2f(rect.getSize().x / 2, rect.getSize().y / 2));

    sf::CircleShape circle(100);

    circle.setFillColor(sf::Color::Green);
    circle.setPosition(viewSize.x / 2, viewSize.y / 2);
    circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));

    sf::ConvexShape triangle;

    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(-100, 0));
    triangle.setPoint(1, sf::Vector2f(0, -100));
    triangle.setPoint(2, sf::Vector2f(100, 0));
    triangle.setFillColor(sf::Color(128, 0, 128, 255));
    triangle.setPosition(viewSize.x / 2, viewSize.y / 2);

	while (window.isOpen()) {
        window.clear(sf::Color::Red);

        window.draw(rect);
        window.draw(circle);
        window.draw(triangle);
        
        window.display();
	}

	return 0;
}