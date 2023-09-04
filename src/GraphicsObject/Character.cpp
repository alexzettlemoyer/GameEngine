#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GraphicsObject.h"
#include "Character.h"
#include "../graphics/draw.hpp"
#include "../Movement/Collider.hpp"
#include <iostream>

static const std::string IMG_CHARACTER = "images/girl.png";
const float movementSpeed = 300.f;
// sf::Vector2f velocity;

// Constructor
Character::Character()
{
    if (!loadTexture(characterTexture, IMG_CHARACTER.c_str()))
        {}          // TODO: handle error
    this -> setTexture(characterTexture);

    sf::Sprite::setScale(sf::Vector2f(0.45f, 0.5f));
    sf::Sprite::setPosition(100.f, 180.f);
    velocity.x = 0.f;
    velocity.y = 0.f;
}

bool Character::loadTexture(sf::Texture& texture, const std::string& image)
{
    if (!texture.loadFromFile(image))
        return false;
    return true;
}

sf::Vector2f Character::getVelocity()
{    
    return sf::Vector2f(velocity.x, velocity.y);
}
sf::Vector2f Character::getPosition()
{
    return sf::Sprite::getPosition();
}
sf::Vector2f Character::getSize()
{
    sf::FloatRect bounds = sf::Sprite::getGlobalBounds();
    return sf::Vector2f(bounds.width, bounds.height);
}
void Character::move(float x, float y)
{
    sf::Sprite::move(x, y);
}

bool Character::checkBounds(int dir, sf::RenderWindow* window)
{
    sf::Vector2u wSize = (*window).getSize();
    sf::Vector2f position = sf::Sprite::getPosition();
    sf::Rect<float> spriteRect = (*this).getGlobalBounds();
            
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
            if ( position.x + spriteRect.width + 1 >= wSize.x )
                return false;
            break;
        case 3:     // check down
            if ( position.y + spriteRect.height + 1 >= wSize.y )
                return false;
            break;
        default:
            return false;
            break;
    }

    //  if any of the objects collide
    for (std::shared_ptr<GraphicsObject> const& i : graphicsObjects) {
        if (checkCollision(*character, *i, 0.0f))
            return false;
    }

    return true;
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

void Character::blockMove()
{
    velocity.x = 0.f;
    velocity.y = 0.f;    
}

void Character::updateMovement()
{
    this -> move(velocity.x, velocity.y);
    velocity.x = 0.f;
    velocity.y = 0.f;
}
