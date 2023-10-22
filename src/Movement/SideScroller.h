#ifndef SIDE_SCROLLER_H
#define SIDE_SCROLLER_H

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
};

#endif