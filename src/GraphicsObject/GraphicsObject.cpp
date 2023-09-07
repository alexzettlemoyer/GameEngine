#include <SFML/Graphics.hpp>
#include "GraphicsObject.h"
#include "../graphics/draw.hpp"
#include "../Movement/Collider.hpp"
#include <iostream>

const float movementSpeed = 200.f;

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

void GraphicsObject::left(float dt)
{
    velocity.x += -movementSpeed * dt;
    if ((*this).checkBounds(0))
        updateMovement();
    else if (!collisionResponse(*this))
        {} // TODO: handle error
    else
        {} // TODO: handle error
}
void GraphicsObject::up(float dt)
{
    velocity.y += -movementSpeed * dt;
    if ((*this).checkBounds(1))
        updateMovement();
    else if (!collisionResponse(*this))
        {} // TODO: handle error
    else
        {} // TODO: handle error
}
void GraphicsObject::right(float dt)
{
    velocity.x += movementSpeed * dt;
    if ((*this).checkBounds(2))
        updateMovement();
    else if (!collisionResponse(*this))
        {} // TODO: handle error
    else
        {} // handle error
}
void GraphicsObject::down(float dt)
{
    this -> velocity.y += movementSpeed * dt;
    if ((*this).checkBounds(3))
        updateMovement();
    else if (!collisionResponse(*this))
        {
            std::cout << "Error with Collision" << std::endl;
        } // TODO: handle error
    else
        {} // handle error
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

