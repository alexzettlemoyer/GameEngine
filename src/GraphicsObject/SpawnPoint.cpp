#include "SpawnPoint.h"
#include "Character.h"


SpawnPoint::SpawnPoint()
{
    this -> position = sf::Vector2f(100.f, 0.f);
}

sf::Vector2f SpawnPoint::getPosition()
{
    return position;
}
