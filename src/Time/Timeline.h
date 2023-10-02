#ifndef TIMELINE_H
#define TIMELINE_H

#include <atomic>

class Timeline
{
    private:
        Timeline();
        static Timeline* instancePtr;
        float dt;
        float ticSize;
        sf::Clock dt_clock;
        std::atomic<bool> paused;

    public:
        enum SCALE { SCALE_HALF, SCALE_REAL, SCALE_DOUBLE };
        static Timeline *getInstance();
        void pause();
        bool isPaused();
        float getDt();
        float getTicSize();
        void editTicSize(float s);
        sf::Time elapsedTime();
        void updateDeltaTime();
};

#endif