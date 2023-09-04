#include <SFML/Graphics.hpp>
#include "Platform.h"


static const sf::Color COLOR_PLATFORM = sf::Color(114.f, 116.f, 122.f);
static const sf::Vector2f SIZE_PLATFORM = sf::Vector2f(425.f, 25.f);

Platform::Platform(const sf::Vector2f& position)
{
    // Constructor
    sf::RectangleShape::setPosition(position);
    (*this).setFillColor(COLOR_PLATFORM);
    (*this).setSize(SIZE_PLATFORM);
}