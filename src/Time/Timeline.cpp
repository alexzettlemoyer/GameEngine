#include <SFML/System/Clock.hpp>
#include <cstddef>
#include <mutex>
#include <chrono>
#include "Timeline.h"
#include <iostream>

std::mutex timeMutex;

Timeline::Timeline()
{
    paused = false;
    ticSize = 1.0;
    startTime = std::chrono::system_clock::now();
    lastTime = std::chrono::system_clock::now();
}

void Timeline::updateDeltaTime()
{
    std::lock_guard<std::mutex> lock(timeMutex);
    if ( anchor == NULL )
    {
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> duration = currentTime - lastTime;

        lastTime = currentTime;
        dt = duration.count();
    }
    else
        anchor -> updateDeltaTime();
}

float Timeline::getDt()
{
    if (anchor != NULL)
        return anchor -> getDt();

    return dt;
}

float Timeline::getTicSize()
{
    // if the game is paused, the time scale is 0.0
    if (paused)
        return 0.0;
        
    return ticSize;
}

void Timeline::editTicSize(float s)
{
    std::lock_guard<std::mutex> lock(timeMutex);
    ticSize = s;
}

bool Timeline::isPaused()
{
    return paused;
}

/**
 * pauses or unpauses the timeline
 */
void Timeline::pause()
{
    std::lock_guard<std::mutex> lock(timeMutex);
    if (paused)
        paused = false;
    else
        paused = true;
}

void Timeline::setAnchor(Timeline* anchor)
{
    this -> anchor = anchor;
}
