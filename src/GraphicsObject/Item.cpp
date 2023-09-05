#include <SFML/Graphics.hpp>
#include "Item.h"
#include "GraphicsObject.h"

static const std::string IMG_MONEY = "images/money.png";

Item::Item(const sf::Vector2f& size, const sf::Vector2f& position)
    : GraphicsObject(size, position)
{
    if (!loadTexture(moneyTexture, IMG_MONEY.c_str()))
    {}                              // TODO: handle error
    this -> setTexture(&moneyTexture);
}