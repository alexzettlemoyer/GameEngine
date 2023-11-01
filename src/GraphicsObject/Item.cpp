#include <SFML/Graphics.hpp>
#include "Item.h"
#include "GraphicsObject.h"

static const std::string IMG_MONEY = "images/money.png";
static const sf::Vector2f SIZE_ITEM = sf::Vector2f(100.2f, 64.6f);
const float movementSpeed = 200.f;

Item::Item(const sf::Vector2f& position, int idNum, std::shared_ptr<Timeline> timeline)
    : GraphicsObject(SIZE_ITEM, position, false, idNum, timeline)
{
    moneyTexture = std::make_shared<sf::Texture>();
    if (!loadTexture(*moneyTexture, IMG_MONEY.c_str()))
        { }                   // TODO: handle error
    this -> setTexture(moneyTexture.get());
    this -> setPosition(position);
    collisionTypeX = ERASE;
    collisionTypeY = ERASE;
}
