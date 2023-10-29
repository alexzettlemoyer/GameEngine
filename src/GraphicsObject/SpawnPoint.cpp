#include "SpawnPoint.h"
#include "Character.h"

static const sf::Vector2f SIZE_SPAWN_POINT = sf::Vector2f(1.f, 1.f);
static const sf::Vector2f POSITION = sf::Vector2f(100.f, 0.f);

SpawnPoint::SpawnPoint()
    : GraphicsObject( SIZE_SPAWN_POINT, POSITION, false, -1, NULL)
{ }

sf::Vector2f SpawnPoint::getPosition()
{
    return GraphicsObject::getPosition();
}
