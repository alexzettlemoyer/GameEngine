#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "GraphicsObject.h"

static const std::string IMG_PLATFORM = "images/platform.jpg";
static const sf::Vector2f SIZE_PLATFORM = sf::Vector2f(425.f, 28.f);
const float movementSpeed = 200.f;

Platform::Platform(const sf::Vector2f& position, int idNum, Timeline* timeline)
    : GraphicsObject(SIZE_PLATFORM, position, true, idNum, timeline)
{
    if (!loadTexture(platformTexture, IMG_PLATFORM.c_str()))
        {} // TODO: handle error
    
    // Constructor
    this -> setTexture(&platformTexture);
    collisionTypeX = STOP_MOVEMENT;
    collisionTypeY = NONE;
}
