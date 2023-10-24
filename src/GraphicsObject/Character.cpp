#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <thread>
#include "GraphicsObject.h"
#include "Character.h"
#include "../Draw/Draw.hpp"
#include "../Movement/Collider.hpp"
#include "../Time/Timeline.h"
#include "../GameRunner/GameState.h"
#include "../Movement/SideScroller.h"
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
    distanceTravelled = 0.f;
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

        if ( velocity.y > 0 )   // character is moving down
            velocity.y = 0;     // but they're on a ground -> do not allow

        velocity.x += ground -> getVelocity().x;
        velocity.y += ground -> getVelocity().y;

        // sometimes the character moves along an up/down moving platform and add the platforms' velocity
        // but the rendering is delayed, so it appears that the character is standing below the platform
        // to fix this, keep the characters' velocity > the platforms'
        if ( velocity.y > 1.f )
            velocity.y -= 1.f;
    }
    if (!checkBounds())
        move(velocity.x, velocity.y);

    this -> blockMove();
}

void Character::respawn()
{
    this -> spawnPoint = new SpawnPoint();
    // SideScroller::getInstance() -> reset();
    distanceTravelled = 0.f;
    this -> setPosition(spawnPoint -> getPosition());
    this -> velocity = sf::Vector2f(0.f, 0.f);
}

bool Character::checkBounds()
{
    // window size = 1000x, 800y
    sf::Vector2f position = sf::RectangleShape::getPosition();
    sf::Rect<float> thisRect = (*this).getGlobalBounds();

    // check up
    if ( thisRect.top < 0.f )
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        this -> setPosition(position.x, 0.1f);
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
        // GameState::getInstance() -> respawn( id );
        respawn();
        return false;
    }
    return false;
}
