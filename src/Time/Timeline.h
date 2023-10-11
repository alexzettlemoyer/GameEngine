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
        static constexpr double SCALE_HALF = 0.5;
        static constexpr double SCALE_REAL = 1.0;
        static constexpr double SCALE_DOUBLE = 2.0;
        static Timeline *getInstance();
        void pause();
        bool isPaused();
        float getDt();
        float getTicSize();
        void editTicSize(float s);
        void updateDeltaTime();
        void setAnchor(Timeline* anchor);
};

#endif