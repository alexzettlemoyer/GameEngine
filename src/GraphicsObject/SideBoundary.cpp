#include "SideBoundary.h"

    // window size = 1000x, 800y
const static sf::Vector2f SIZE_SIDE_BOUNDARY = sf::Vector2f(800.f, 0.f);

SideBoundary::SideBoundary(const sf::Vector2f& position, int idNum, Timeline* timeline)
    : GraphicsObject(SIZE_SIDE_BOUNDARY, position, false, idNum, timeline)
{
    this -> setPosition(position);
    collisionTypeX = SCROLL;
    collisionTypeY = SCROLL;
}