#include <SFML/System/Clock.hpp>
#include <cstddef>
#include "Timeline.h"
#include <iostream>

// TODO: 
// - support an adjustable tic size
// - enable anchoring your timeline to another, arbitrary timeline (or to some measure of real time)


// singleton TimeHandler instance
Timeline* Timeline::instancePtr = nullptr;
bool paused;

// Singleton Time class
Timeline::Timeline()
    {
        paused = false;
        scale = 1.0;
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
    dt = dt_clock.restart().asSeconds();
}

sf::Time Timeline::elapsedTime()
{
    return dt_clock.getElapsedTime();
}

float Timeline::getDt()
{
    return dt;
}

float Timeline::getScale()
{
    // if the game is paused, the time scale is 0.0
    if (paused)
        return 0.0;
    return scale;
}

void Timeline::changeScale(SCALE s)
{
    switch (s)
    {
        case SCALE_HALF:
            scale = 0.5;
            break;
        case SCALE_REAL:
            scale = 1.0;
            break;
        case SCALE_DOUBLE:
            scale = 2.0;
            break;
    }
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
    if (paused)
        paused = false;
    else
        paused = true;
}
