#ifndef SIDE_BOUNDARY_H
#define SIDE_BOUNDARY_H

#include "GraphicsObject.h"
#include "../Time/Timeline.h"

class SideBoundary : public GraphicsObject
{
    private:
        
    public:
        SideBoundary(const sf::Vector2f& position, int idNum, Timeline* timeline);
        int getType() const override { return GraphicsObject::SIDE_BOUNDARY_TYPE; }
};

#endif