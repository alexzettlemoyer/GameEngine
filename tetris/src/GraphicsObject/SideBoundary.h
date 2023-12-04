#ifndef SIDE_BOUNDARY_H
#define SIDE_BOUNDARY_H

#include "GraphicsObject.h"
#include "../Time/Timeline.h"

class SideBoundary : public GraphicsObject
{
    private:
        int direction;

    public:
        SideBoundary(const sf::Vector2f& position, int idNum, std::shared_ptr<Timeline> timeline, int direction);
        int getType() const override { return GraphicsObject::SIDE_BOUNDARY_TYPE; }
        int getDirection();
        static inline const int RIGHT = 0;
        static inline const int LEFT = 1;
};

#endif