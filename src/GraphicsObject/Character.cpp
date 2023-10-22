#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GraphicsObject.h"
#include "Character.h"
#include "../Draw/Draw.hpp"
#include "../Movement/Collider.hpp"
#include "../Time/Timeline.h"
#include "../GameRunner/GameState.h"
#include <iostream>

static const std::string IMG_CHARACTER = "images/girl.png";
static const sf::Vector2f SIZE_CHARACTER = sf::Vector2f(116.f, 256.f);
static sf::Vector2u wSize = sf::Vector2u(1000, 800);

static const float displacement = .085f;
static const float acceleration = -1600.f;
static const float GRAVITY = 300.f;

sf::Vector2f initialPosition;

// Constructor
Character::Character(sf::Vector2f position, int idNum, Timeline* timeline, SpawnPoint *spawnPoint)
    : GraphicsObject(SIZE_CHARACTER, position, false, idNum, timeline)
{
    if (!loadTexture(characterTexture, IMG_CHARACTER.c_str()))
        {}          // TODO: handle error
    this -> setTexture(&characterTexture);
    this -> collisionTypeX = CHAR;
    this -> collisionTypeY = CHAR;
    this -> spawnPoint = spawnPoint;
}

void Character::left()
{
    float dt = timeline -> getDt();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (-displacement / dt) * timeline -> getTicSize();
    }
    updateMovement();
}
void Character::up()
{
    float dt = timeline -> getDt();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y = (acceleration * dt) * timeline -> getTicSize();
    }
    updateMovement();
}
void Character::right()
{
    float dt = timeline -> getDt();
    { 
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (displacement / dt) * timeline -> getTicSize();
    }
    updateMovement();
}
void Character::down()
{
    float dt = timeline -> getDt();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (displacement / dt) * timeline -> getTicSize();
    }
    updateMovement();
}

std::shared_ptr<GraphicsObject> Character::isGrounded()
{
    //  if the character is on top of any of the platforms
    for (std::shared_ptr<GraphicsObject> const& i : (GameState::getInstance() -> getGraphicsObjects()))
    {
        if (i -> isGround() && isCharacterGrounded(*this, *i))
            return i;
    }

    return nullptr;
}

void Character::updateMovement()
{
    std::shared_ptr<GraphicsObject> ground = isGrounded();
    if (ground == nullptr)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (GRAVITY * timeline -> getDt()) * timeline -> getTicSize();
    }
    else 
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += ground -> getVelocity().x;
    }
    if (!checkBounds())
        move(velocity.x, velocity.y);

    this -> blockMove(false);
}

void Character::respawn()
{
    this -> spawnPoint = new SpawnPoint();
    this -> setPosition(spawnPoint -> getPosition());
}

bool Character::checkBounds()
{
    // window size = 1000x, 800y
    sf::Vector2f position = sf::RectangleShape::getPosition();
    sf::Rect<float> thisRect = (*this).getGlobalBounds();

    // check left
    if ( thisRect.left < 0.f )
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        this -> velocity.x = 1.f;
        return false;
    }
    // check up
    if ( thisRect.top < 0.f )
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        this -> setPosition(position.x, 0.1f);
        return false;
    }
    // check right
    if ( thisRect.left + thisRect.width + 1 >= wSize.x )
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        this -> velocity.x = -1.f;
        return false;
    }

    //  if any of the objects collide
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects())
    {
        // check if the graphicsObject is this character
        // if its not the same character (has the same ID), check for collisions
        if (i.get() -> identifier() != this -> identifier())
        {
            checkCollision(*this, *i);
        }
    }

        // check if we're colliding with the death zone
    if (checkCollision(*this, *(GameState::getInstance() -> getDeathZone())))
    {
        respawn();
        return false;
    }

        // check if we're colliding with any sideBoundaries
    for (std::shared_ptr<SideBoundary> const& i : GameState::getInstance() -> getSideBoundaries())
    {
        checkCollision(*this, *i);
    }

    return false;
}
