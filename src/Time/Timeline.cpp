#include <SFML/System/Clock.hpp>
#include <cstddef>
#include <mutex>
#include "Timeline.h"

// TODO: 
// - enable anchoring your timeline to another, arbitrary timeline (or to some measure of real time)

std::mutex timeMutex;

// singleton TimeHandler instance
Timeline* Timeline::instancePtr = nullptr;

// Singleton Time class
Timeline::Timeline()
    {
        paused = false;
        ticSize = 1.0;
    }

Timeline* Timeline::getInstance()
{
    if ( instancePtr == NULL )
    {
        instancePtr = new Timeline();
    }
    return instancePtr;
}

void Timeline::updateDeltaTime()
{
    std::lock_guard<std::mutex> lock(timeMutex);
    dt = dt_clock.restart().asSeconds();
}

sf::Time Timeline::elapsedTime()
{
    std::lock_guard<std::mutex> lock(timeMutex);
    return dt_clock.getElapsedTime();
}

float Timeline::getDt()
{
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
