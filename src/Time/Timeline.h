#ifndef TIMELINE_H
#define TIMELINE_H

#include <atomic>
#include <chrono>

class Timeline
{
    private:
        float dt;
        float ticSize;
        sf::Clock dt_clock;
        std::atomic<bool> paused;
        Timeline* anchor;
        std::chrono::time_point<std::chrono::system_clock> startTime;
        std::chrono::time_point<std::chrono::system_clock> lastTime;

    public:
        Timeline();
        enum SCALE { SCALE_HALF, SCALE_REAL, SCALE_DOUBLE };
        static Timeline *getInstance();
        void pause();
        bool isPaused();
        float getDt();
        float getTicSize();
        void editTicSize(float s);
        // sf::Time elapsedTime();
        void updateDeltaTime();
        void setAnchor(Timeline* anchor);
};

#endif