#include <SFML/Graphics.hpp>
#include <mutex>
#include "GraphicsObject.h"
#include "../Draw/Draw.hpp"
#include "../Movement/Collider.hpp"
#include "../Time/Timeline.h"

const float displacement = .015f;

GraphicsObject::GraphicsObject(sf::Vector2f size, sf::Vector2f position)
{
    this -> setSize(size);
    this -> setPosition(position);

    velocity.x = 0.f;
    velocity.y = 0.f;
}

bool GraphicsObject::loadTexture(sf::Texture& texture, const std::string& image)
{
    if (!texture.loadFromFile(image))
        return false;
    return true;
}

sf::Vector2f GraphicsObject::getPosition()
{
    return sf::RectangleShape::getPosition();
}

sf::Vector2f GraphicsObject::getSize()
{
    return sf::RectangleShape::getSize();
}

void GraphicsObject::move(float x, float y)
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    sf::RectangleShape::move(x, y);
}

sf::Vector2f GraphicsObject::getVelocity()
{    
    return sf::Vector2f(velocity.x, velocity.y);
}

void GraphicsObject::left()
{
    if (Timeline::getInstance() -> dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += -displacement / Timeline::getInstance() -> dt;
    }
    (*this).checkBounds();
    updateMovement();
}
void GraphicsObject::up()
{
    if (Timeline::getInstance() -> dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += -displacement / Timeline::getInstance() -> dt;
    }
    (*this).checkBounds();
    updateMovement();
}
void GraphicsObject::right()
{
    if (Timeline::getInstance() -> dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += displacement / Timeline::getInstance() -> dt;
    }    
    (*this).checkBounds();
    updateMovement();
}
void GraphicsObject::down()
{
    if (Timeline::getInstance() -> dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += displacement / Timeline::getInstance() -> dt;
    }    
    (*this).checkBounds();
    updateMovement();
}

void GraphicsObject::blockMove()
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    velocity.x = 0.f;
    velocity.y = 0.f;  
}

void GraphicsObject::updateMovement()
{
    this -> move(velocity.x, velocity.y);
    this -> blockMove();
}

bool GraphicsObject::checkBounds()
{
    if (checkCollision(*((Draw::getInstance() -> character).get()), *this))
        return false;
    return true;
}

