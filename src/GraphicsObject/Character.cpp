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
