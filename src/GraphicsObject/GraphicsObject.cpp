#include <SFML/Graphics.hpp>
#include "GraphicsObject.h"
#include "../graphics/draw.hpp"
#include "../Movement/Collider.hpp"
#include <iostream>

const float movementSpeed = 300.f;

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

void GraphicsObject::left(sf::RenderWindow* window, float dt)
{
    velocity.x += -movementSpeed * dt;
    if ((*this).checkBounds(0, window))
        updateMovement();
    else
        blockMove();
}
void GraphicsObject::up(sf::RenderWindow* window, float dt)
{
    velocity.y += -movementSpeed * dt;
    if ((*this).checkBounds(1, window))
        updateMovement();
    else
        blockMove();
}
void GraphicsObject::right(sf::RenderWindow* window, float dt)
{
    velocity.x += movementSpeed * dt;
    if ((*this).checkBounds(2, window))
        updateMovement();
    else
        blockMove();
}
void GraphicsObject::down(sf::RenderWindow* window, float dt)
{
    velocity.y += movementSpeed * dt;
    if ((*this).checkBounds(3, window))
        updateMovement();
    else
        blockMove();
}

void GraphicsObject::blockMove()
{
    velocity.x = 0.f;
    velocity.y = 0.f;    
}

void GraphicsObject::updateMovement()
{
    this -> move(velocity.x, velocity.y);
    velocity.x = 0.f;
    velocity.y = 0.f;
}

bool GraphicsObject::checkBounds(int dir, sf::RenderWindow* window)
{
    sf::Vector2u wSize = (*window).getSize();
    sf::Vector2f position = sf::RectangleShape::getPosition();
    sf::Rect<float> thisRect = (*this).getGlobalBounds();
            
    switch ( dir )
    {
        case 0:     // check left
            if ( position.x < 0.f )
                return false;
            break;
        case 1:     // check up
            if ( position.y < 0.f )
                return false;
            break;
        case 2:     // check right
            if ( position.x + thisRect.width + 1 >= wSize.x )
                return false;
            break;
        case 3:     // check down
            if ( position.y + thisRect.height + 1 >= wSize.y )
                return false;
            break;
        default:
            return false;
            break;
    }

    //  if any of the objects collide
    for (std::shared_ptr<GraphicsObject> const& i : graphicsObjects) {
        if (checkCollision(*this, *i, 0.0f))
            return false;
    }

    return true;
}

