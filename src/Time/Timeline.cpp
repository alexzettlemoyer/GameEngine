#include <SFML/System/Clock.hpp>
#include <cstddef>
#include "Timeline.h"
#include <iostream>

// singleton TimeHandler instance
Timeline* Timeline::instancePtr = nullptr;
bool paused;

// Singleton Time class
Timeline::Timeline()
    {
        paused = false;
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
    {
        std::cout << "Unpausing" << std::endl;
        paused = false;
    }
    else
    {
        std::cout << "Pausing" << std::endl;
        paused = true;
    }
}
