#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GraphicsObject.h"
#include "Character.h"
#include "../graphics/draw.hpp"
#include "../Movement/Collider.hpp"
#include <iostream>

static const std::string IMG_CHARACTER = "images/girl.png";
static const sf::Vector2f SIZE_CHARACTER = sf::Vector2f(154.f, 340.f);

const float movementSpeed = 300.f;

// Constructor
Character::Character(sf::Vector2f position)
    : GraphicsObject(SIZE_CHARACTER, position)
{
    if (!loadTexture(characterTexture, IMG_CHARACTER.c_str()))
        {}          // TODO: handle error
    this -> setTexture(&characterTexture);
}

void Character::left(sf::RenderWindow* window, float dt)
{
    velocity.x += -movementSpeed * dt;
    if ((*this).checkBounds(0, window))
        updateMovement();
    else
        blockMove();
}
void Character::up(sf::RenderWindow* window, float dt)
{
    velocity.y += -movementSpeed * dt;
    if ((*this).checkBounds(1, window))
        updateMovement();
    else
        blockMove();
}
void Character::right(sf::RenderWindow* window, float dt)
{
    velocity.x += movementSpeed * dt;
    if ((*this).checkBounds(2, window))
        updateMovement();
    else
        blockMove();
}
void Character::down(sf::RenderWindow* window, float dt)
{
    velocity.y += movementSpeed * dt;
    if ((*this).checkBounds(3, window))
        updateMovement();
    else
        blockMove();
}

bool Character::checkBounds(int dir, sf::RenderWindow* window)
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
