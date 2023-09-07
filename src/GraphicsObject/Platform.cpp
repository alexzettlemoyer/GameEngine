#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "GraphicsObject.h"


static const sf::Color COLOR_PLATFORM = sf::Color(114.f, 116.f, 122.f);
static const sf::Vector2f SIZE_PLATFORM = sf::Vector2f(425.f, 25.f);
const float movementSpeed = 200.f;

Platform::Platform(const sf::Vector2f& position)
    : GraphicsObject(SIZE_PLATFORM, position)
{

    // Constructor
    (*this).setFillColor(COLOR_PLATFORM);
    // collisionType = PUSH;
    collisionType = STOP_MOVEMENT;
}