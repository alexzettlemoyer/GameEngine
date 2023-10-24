#ifndef SIDE_SCROLLER_H
#define SIDE_SCROLLER_H

#include "../GraphicsObject/GraphicsObject.h"

class SideScroller
{
    private:
        SideScroller();
        static SideScroller *instancePtr;
        float sideScrollSpeed = 0.f;
        float totalScrollDistance = 0.f;

    public:
        static SideScroller* getInstance();
        float getSideScrollSpeed();
        float getSideScrollDistance();
        void scrollWindow(int direction);
        bool checkSideCollision(GraphicsObject* character, GraphicsObject* sideBoundary);
        void reset();
        static constexpr float MIN_POSITION = 0.f;
        static constexpr float MAX_POSITION = 2500.f;
};

#endif