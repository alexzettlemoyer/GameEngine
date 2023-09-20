#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GraphicsObject.h"
#include "Character.h"
#include "../Draw/Draw.hpp"
#include "../Movement/Collider.hpp"
#include "../Time/Timeline.h"

static const std::string IMG_CHARACTER = "images/girl.png";
static const sf::Vector2f SIZE_CHARACTER = sf::Vector2f(154.f, 340.f);
static sf::RenderWindow* window;

static const float displacement = .065f;
static const float acceleration = -1850.f;       // m/s * s
static const float GRAVITY = 520.f;            // m/s * s

sf::Vector2f initialPosition;

// Constructor
Character::Character(sf::Vector2f position)
    : GraphicsObject(SIZE_CHARACTER, position)
{
    if (!loadTexture(characterTexture, IMG_CHARACTER.c_str()))
        {}          // TODO: handle error
    this -> setTexture(&characterTexture);
    this -> collisionTypeX = CHAR;
    this -> collisionTypeY = CHAR;
}

void Character::addWindowReference(sf::RenderWindow* windowRef)
{
    window = windowRef;
}

void Character::left()
{
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += -displacement / Timeline::getInstance() -> dt;
    }
    updateMovement();
}
void Character::up()
{
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y = acceleration * Timeline::getInstance() -> dt;
    }
    updateMovement();
}
void Character::right()
{
    { 
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += displacement / Timeline::getInstance() -> dt;
    }
    updateMovement();
}
void Character::down()
{
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += displacement / Timeline::getInstance() -> dt;
    }
    updateMovement();
}

bool Character::isGrounded()
{
    //  if the character is on top of any of the platforms
    for (std::shared_ptr<Platform> const& i : (Draw::getInstance() -> platforms)) {
        if (isCharacterGrounded(*this, *i))
            return true;
    }

    sf::Vector2u wSize = (*window).getSize();
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
        velocity.y += GRAVITY * Timeline::getInstance()->dt;
    }
    if (!checkBounds())
        move(velocity.x, velocity.y);
    this -> blockMove();
}

bool Character::checkBounds()
{
    sf::Vector2u wSize = (*window).getSize();
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
    for (std::shared_ptr<GraphicsObject> const& i : Draw::getInstance() -> graphicsObjects) {
        if (checkCollision(*this, *i))
            return true;
    }

    return false;
}
