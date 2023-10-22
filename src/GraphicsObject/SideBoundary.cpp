#include "SideBoundary.h"

    // window size = 1000x, 800y
const static sf::Vector2f SIZE_SIDE_BOUNDARY = sf::Vector2f(150.f, 800.f);
// const int SideBoundary::RIGHT = 0;
// const int SideBoundary::LEFT = 1;

SideBoundary::SideBoundary(const sf::Vector2f& position, int idNum, Timeline* timeline, int direction)
    : GraphicsObject(SIZE_SIDE_BOUNDARY, position, false, idNum, timeline)
{
    this -> direction = direction;
    this -> setPosition(position);
    collisionTypeX = SCROLL;
    collisionTypeY = SCROLL;
}

int SideBoundary::getDirection()
{
    return direction;
}