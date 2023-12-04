#ifndef SPAWN_POINT_H
#define SPAWN_POINT_H

#include "SpawnPoint.h"
#include "GraphicsObject.h"

class SpawnPoint : public GraphicsObject
{
    public:
        SpawnPoint();
        int getType() const override { return GraphicsObject::SPAWN_POINT_TYPE; }
        sf::Vector2f getPosition();
};

#endif