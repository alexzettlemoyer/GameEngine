#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "GraphicsObject.h"

static const std::string IMG_PLATFORM = "images/platform.jpg";
static const sf::Vector2f SIZE_PLATFORM = sf::Vector2f(256.f, 10.f);
const float movementSpeed = 200.f;

std::vector<sf::Color> colors = { sf::Color(210, 250, 212), sf::Color(sf::Color(210, 211, 250)), sf::Color(247, 210, 250) };

Platform::Platform(const sf::Vector2f& position, int idNum, Timeline* timeline, int colorNum)
    : GraphicsObject(SIZE_PLATFORM, position, true, idNum, timeline)
{
    // if (!loadTexture(platformTexture, IMG_PLATFORM.c_str()))
    //     {}
    
    // this -> setTexture(&platformTexture);

    this -> setFillColor(colors[ colorNum ]);
    this -> setOutlineColor(sf::Color::Black);
    this -> setOutlineThickness(2.f);
    this -> setPosition(position);
    collisionTypeX = STOP_MOVEMENT;
    collisionTypeY = NONE;
}
