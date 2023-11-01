#ifndef DEATH_ZONE_H
#define DEATH_ZONE_H

#include "GraphicsObject.h"
#include "../Time/Timeline.h"

class DeathZone : public GraphicsObject
{
    private:
        
    public:
        DeathZone(const sf::Vector2f& position, int idNum, std::shared_ptr<Timeline> timeline);
        int getType() const override { return GraphicsObject::DEATHZONE_TYPE; }
};

#endif