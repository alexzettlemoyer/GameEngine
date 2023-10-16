#ifndef SPAWN_POINT_H
#define SPAWN_POINT_H

#include "SpawnPoint.h"
#include "GraphicsObject.h"

class SpawnPoint
{
    private:
        sf::Vector2f position;

    public:
        SpawnPoint();
        sf::Vector2f getPosition();
};

#endif