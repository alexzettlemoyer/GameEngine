#include <SFML/Graphics.hpp>
#include "GraphicsObject.h"
#include "Character.h"
#include "../graphics/draw.hpp"
#include "../Movement/Collider.hpp"

static const std::string IMG_CHARACTER = "images/girl.png";

// Constructor
Character::Character()
{
    if (!loadTexture(characterTexture, IMG_CHARACTER.c_str()))
        {}          // TODO: handle error
    this -> setTexture(characterTexture);

    sf::Sprite::setScale(sf::Vector2f(0.45f, 0.5f));
    sf::Sprite::setPosition(100.f, 173.f);
}

bool Character::loadTexture(sf::Texture& texture, const std::string& image)
{
    if (!texture.loadFromFile(image))
        return false;
    return true;
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
            if ( position.x - 1 <= 0 )
                return false;
            break;
        case 1:     // check up
            if ( position.y - 1 <= 0 )
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
            return true;
        break;
    }

    checkCollision(*character, *platform1, 0.0f);
    checkCollision(*character, *platform2, 0.0f);
    checkCollision(*character, *item1, 0.0f);


    return true;
}

void Character::left(sf::RenderWindow* window)
{
    if ((*this).checkBounds(0, window))
        sf::Sprite::move(-3.f, 0.f);
}
void Character::up(sf::RenderWindow* window)
{
    if (checkBounds(1, window))
        sf::Sprite::move(0.f, -3.f);
}
void Character::right(sf::RenderWindow* window)
{
    if (checkBounds(2, window))
        sf::Sprite::move(3.f, 0.f);
}
void Character::down(sf::RenderWindow* window)
{
    if (checkBounds(3, window))
        sf::Sprite::move(0.f, 3.f);
}
