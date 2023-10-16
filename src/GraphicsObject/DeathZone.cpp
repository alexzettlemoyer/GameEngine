#include "DeathZone.h"

    // window size = 1000x, 800y
const static sf::Vector2f SIZE_DEATH_ZONE = sf::Vector2f(1000.f, 80.f);

DeathZone::DeathZone(const sf::Vector2f& position, int idNum, Timeline* timeline)
    : GraphicsObject(SIZE_DEATH_ZONE, position, false, idNum, timeline)
{
    this -> setPosition(position);
    collisionTypeX = DEATH;
    collisionTypeY = DEATH;
}