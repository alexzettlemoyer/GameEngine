#ifndef CHARACTER_H
#define CHARACTER_H

#include "GraphicsObject.h"
#include "SpawnPoint.h"
#include "../Time/Timeline.h"
#include <memory>


class Character : public GraphicsObject
{
    private:
        std::shared_ptr<sf::Texture> characterTexture;
        std::shared_ptr<GraphicsObject> isGrounded();
        std::shared_ptr<SpawnPoint> spawnPoint;
        bool checkBounds();
        bool respawned;

    public:
        Character(sf::Vector2f position, int idNum, std::shared_ptr<Timeline> timeline, std::shared_ptr<SpawnPoint> spawnPoint = nullptr);
        void updateMovement();
        void left();
        void up();
        void right();
        void down();
        void respawn();
        bool wasRespawned();
        sf::Vector2f getVelocity();
        int getType() const override { return GraphicsObject::CHARACTER_TYPE; }
};

#endif