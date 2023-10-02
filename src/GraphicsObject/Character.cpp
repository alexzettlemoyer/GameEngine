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
static const sf::Vector2f SIZE_CHARACTER = sf::Vector2f(154.f, 340.f);
// static sf::RenderWindow* window;
static sf::Vector2u wSize = sf::Vector2u(1000, 800);

static const float displacement = .085f;            // .085
static const float acceleration = -1850.f;       // m/s * s
static const float GRAVITY = 600.f;            // m/s * s   // 600

sf::Vector2f initialPosition;

// Constructor
Character::Character(sf::Vector2f position, int idNum)
    : GraphicsObject(SIZE_CHARACTER, position, false, idNum)
{
    if (!loadTexture(characterTexture, IMG_CHARACTER.c_str()))
        {}          // TODO: handle error
    this -> setTexture(&characterTexture);
    this -> collisionTypeX = CHAR;
    this -> collisionTypeY = CHAR;
}

void Character::left()
{
    float dt = Timeline::getInstance() -> getDt();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (-displacement / dt) * Timeline::getInstance() -> getTicSize();
    }
    updateMovement();
}
void Character::up()
{
    float dt = Timeline::getInstance() -> getDt();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y = (acceleration * dt) * Timeline::getInstance() -> getTicSize();
    }
    updateMovement();
}
void Character::right()
{
    float dt = Timeline::getInstance() -> getDt();
    { 
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (displacement / dt) * Timeline::getInstance() -> getTicSize();
    }
    updateMovement();
}
void Character::down()
{
    float dt = Timeline::getInstance() -> getDt();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (displacement / dt) * Timeline::getInstance() -> getTicSize();
    }
    updateMovement();
}

bool Character::isGrounded()
{
    //  if the character is on top of any of the platforms
    // for (std::shared_ptr<GraphicsObject> const& i : Draw::getInstance() -> graphicsObjects)
    for (std::shared_ptr<GraphicsObject> const& i : (GameState::getInstance() -> getGraphicsObjects()))
    {
        if (i -> isGround() && isCharacterGrounded(*this, *i))
            return true;
    }

    // sf::Vector2u wSize = (*window).getSize();
    sf::FloatRect characterBounds = this -> getGlobalBounds();

    // if bottom x coordinate of character is below top x coordinate of window
    // and the character is within the x coordinates of the platform ( on top )
    if (characterBounds.top + characterBounds.height >= wSize.y)
            return true;

    return false;
}

void Character::updateMovement()
{
    if (!isGrounded())
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (GRAVITY * Timeline::getInstance() -> getDt()) * Timeline::getInstance() -> getTicSize();
    }
    if (!checkBounds())
        move(velocity.x, velocity.y);
    this -> blockMove();
}

bool Character::checkBounds()
{

    // window size = 1000x, 800y
    // sf::Vector2u wSize = sf::Vector2u(1000, 800);
    // sf::Vector2u wSize = (*window).getSize();
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
    // for (std::shared_ptr<GraphicsObject> const& i : Draw::getInstance() -> graphicsObjects)
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects())
    {
        // check if the graphicsObject is this character
        // if its not the same character (has the same ID), check for collisions
        if (i.get() -> identifier() != this -> identifier() && checkCollision(*this, *i))
            return true;
    }

    return false;
}
