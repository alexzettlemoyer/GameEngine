#include <SFML/Graphics.hpp>
#include "Platform.h"


static const sf::Color COLOR_PLATFORM = sf::Color(114.f, 116.f, 122.f);

Platform::Platform(const sf::Vector2f& position)
{
    // Constructor
    sf::RectangleShape::setPosition(position);
    (*this).setFillColor(COLOR_PLATFORM);
    (*this).setSize(sf::Vector2f(450.f, 25.f));
}