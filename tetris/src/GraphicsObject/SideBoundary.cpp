#include "SideBoundary.h"

    // window size = 1000x, 800y
const static sf::Vector2f SIZE_SIDE_BOUNDARY = sf::Vector2f(500.f, 800.f);

SideBoundary::SideBoundary(const sf::Vector2f& position, int idNum, std::shared_ptr<Timeline> timeline, int direction)
    : GraphicsObject(SIZE_SIDE_BOUNDARY, position, false, idNum, timeline)
{
    this -> direction = direction;
    this -> setPosition(position);
    collisionTypeX = SCROLL;
    collisionTypeY = SCROLL;

    this -> setFillColor(sf::Color::Red);
}

int SideBoundary::getDirection()
{
    return direction;
}