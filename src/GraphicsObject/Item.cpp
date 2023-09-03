#include <SFML/Graphics.hpp>
#include "Item.h"
#include "GraphicsObject.h"

static const std::string IMG_MONEY = "images/money.png";

Item::Item(const sf::Vector2f& position)
{
    // Constructor
    if (!loadTexture(moneyTexture, IMG_MONEY.c_str()))
    {}                              // TODO: handle error
    this -> setTexture(&moneyTexture);
    (*this).setSize(position);
    sf::RectangleShape::setPosition(550.f, 200.f);
}