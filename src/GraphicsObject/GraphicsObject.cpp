#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <mutex>
#include "GraphicsObject.h"
#include "../Movement/Collider.hpp"
#include "../Time/Timeline.h"
#include "../GameRunner/ServerGameState.h"
#include <iostream>

const float displacement = .025f;

GraphicsObject::GraphicsObject(sf::Vector2f size, sf::Vector2f position, bool isGround, int idNum, Timeline *timeline)
{
    this -> setSize(size);
    this -> setPosition(position);
    originalPosition = position;
    ground = isGround;

    velocity.x = 0.f;
    velocity.y = 0.f;

    previousVelocity = sf::Vector2f(0.f, 0.f);
    id = idNum;

    this -> timeline = timeline;
}

void GraphicsObject::setMovementFunction(MovementFunction func) {
    movementFunction = func;
}

GraphicsObject::MovementFunction GraphicsObject::getMovementFunction()
{
    return movementFunction;
}

bool GraphicsObject::loadTexture(sf::Texture& texture, const std::string& image)
{
    if (!texture.loadFromFile(image))
        return false;
    return true;
}

int GraphicsObject::identifier()
{
    return id;
}

sf::Vector2f GraphicsObject::getPosition()
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    return sf::RectangleShape::getPosition();
}

sf::Vector2f GraphicsObject::getOriginalPosition()
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    return originalPosition;
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
    if (velocity == sf::Vector2f(0.f, 0.f) && previousVelocity != sf::Vector2f(0.f, 0.f))
        return previousVelocity;
    return sf::Vector2f(velocity.x, velocity.y);
}

void GraphicsObject::left()
{
    float dt = timeline -> getDt();
    if (dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (-displacement / dt) * timeline -> getTicSize();
    }
    (*this).checkBounds();
    updateMovement();
}

void GraphicsObject::up()
{
    float dt = timeline -> getDt();
    if (dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (-displacement / dt) * timeline -> getTicSize();
    }
    (*this).checkBounds();
    updateMovement();
}
void GraphicsObject::right()
{
    float dt = timeline -> getDt();
    if (dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (displacement / dt) * timeline -> getTicSize();
    }
    (*this).checkBounds();
    updateMovement();
}
void GraphicsObject::down()
{
    float dt = timeline -> getDt();
    if (dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (displacement / dt) * timeline -> getTicSize();
    }
    (*this).checkBounds();
    updateMovement();
}

void GraphicsObject::blockMove()
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    this -> previousVelocity = sf::Vector2f(velocity.x, velocity.y);
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
    for (Character* i : ServerGameState::getInstance() -> getCharacters())
    {
        if (checkCollision(*this, *i, true))
            return false;
    }
    return true;
}

bool GraphicsObject::isGround()
{
    return ground;
}
