#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GraphicsObject.h"
#include "Character.h"
#include "../Draw/draw.hpp"
#include "../Movement/Collider.hpp"
#include "../Time/TimeHandler.h"
#include <iostream>

static const std::string IMG_CHARACTER = "images/girl.png";
static const sf::Vector2f SIZE_CHARACTER = sf::Vector2f(154.f, 340.f);
static sf::RenderWindow* window;

static const float displacement = .065f;
static const float acceleration = -1750.f;       // m/s * s
static const float GRAVITY = 500.f;            // m/s * s

sf::Vector2f initialPosition;

// Constructor
Character::Character(sf::Vector2f position)
    : GraphicsObject(SIZE_CHARACTER, position)
{
    if (!loadTexture(characterTexture, IMG_CHARACTER.c_str()))
        {}          // TODO: handle error
    this -> setTexture(&characterTexture);
    this -> collisionType = CHAR;
}

void Character::addWindowReference(sf::RenderWindow* windowRef)
{
    window = windowRef;
}

void Character::left()
{
    velocity.x += -displacement / TimeHandler::getInstance() -> dt;
    updateMovement();
}
void Character::up()
{
    // if (initialPosition == sf::Vector2f(0.f, 0.f))
    //     initialPosition = getPosition();

    velocity.y = acceleration * TimeHandler::getInstance() -> dt;
    updateMovement();
}
void Character::right()
{
    velocity.x += displacement / TimeHandler::getInstance() -> dt;
    updateMovement();
}
void Character::down()
{
    velocity.y += displacement / TimeHandler::getInstance() -> dt;
    updateMovement();
}

bool Character::isGrounded()
{
    //  if the character is on top of any of the platforms
    for (std::shared_ptr<Platform> const& i : platforms) {
        // std::cout << i.get() -> getPosition().x << std::endl; 
        if (isCharacterGrounded(*this, *i))
            return true;
    }
    return false;
}

void Character::updateMovement()
{
    sf::Vector2f currentPosition = getPosition();

    if (!isGrounded())
        velocity.y += GRAVITY * TimeHandler::getInstance()->dt;

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
        this -> setPosition(0.1f, position.y);
    // check up
    if ( thisRect.top < 0.f )
        this -> setPosition(position.x, 0.1f);
    // check right
    if ( thisRect.left + thisRect.width + 1 >= wSize.x )
        this -> setPosition(wSize.x - thisRect.width - 1, position.y);
    // check down
    if ( thisRect.top + thisRect.height + 1 >= wSize.y )
        this -> setPosition(position.x, wSize.y - thisRect.height - 1);

    //  if any of the objects collide
    for (std::shared_ptr<GraphicsObject> const& i : graphicsObjects) {
        if (checkCollision(*this, *i, 0.0f))
        {
            // std::cout << "c" <<  position.y +thisRect.height << " - " << i.get() -> getPosition().y << std::endl;
            return true;
        }
    }

    // std::cout << "not colliding" << position.y << std::endl;
    return false;
}
