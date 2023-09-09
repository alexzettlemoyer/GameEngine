#include <SFML/Graphics.hpp>
#include "GraphicsObject.h"
#include "../Draw/draw.hpp"
#include "../Movement/Collider.hpp"
#include "../Time/TimeHandler.h"
#include <iostream>

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
    sf::RectangleShape::move(x, y);
}

sf::Vector2f GraphicsObject::getVelocity()
{    
    return sf::Vector2f(velocity.x, velocity.y);
}

void GraphicsObject::left()
{
    if (TimeHandler::getInstance() -> dt != 0)
        velocity.x += -displacement / TimeHandler::getInstance() -> dt;
    // std::cout << velocity.x << std::endl;
    // if ((*this).checkBounds(0))
        updateMovement();
    // else if (!collisionResponse(*this))
    //     {} // TODO: handle error
    // else
    //     {} // TODO: handle error
}
void GraphicsObject::up()
{
    if (TimeHandler::getInstance() -> dt != 0)
        velocity.y += -displacement / TimeHandler::getInstance() -> dt;
    // if ((*this).checkBounds(1))
        updateMovement();
    // else if (!collisionResponse(*this))
    //     {} // TODO: handle error
    // else
    //     {} // TODO: handle error
}
void GraphicsObject::right()
{
    if (TimeHandler::getInstance() -> dt != 0)
        velocity.x += displacement / TimeHandler::getInstance() -> dt;
    if ((*this).checkBounds(2))
        updateMovement();
    else if (!collisionResponse(*this))
        {} // TODO: handle error
    else
        {} // handle error
}
void GraphicsObject::down()
{
    if (TimeHandler::getInstance() -> dt != 0)
        velocity.y += displacement / TimeHandler::getInstance() -> dt;
    // if ((*this).checkBounds(3))
        updateMovement();
    // else if (!collisionResponse(*this))
    //     {
    //         std::cout << "Error with Collision" << std::endl;
    //     } // TODO: handle error
    // else
    //     {} // handle error
}

void GraphicsObject::blockMove()
{
    velocity.x = 0.f;
    velocity.y = 0.f;  
}

void GraphicsObject::updateMovement()
{
    this -> move(velocity.x, velocity.y);
    this -> blockMove();
}

bool GraphicsObject::checkBounds(int dir)
{
    if (checkCollision(*this, *character, 0.0f))
        return false;
    return true;
}

