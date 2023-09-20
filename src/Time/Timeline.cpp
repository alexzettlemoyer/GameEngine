#include <SFML/System/Clock.hpp>
#include <cstddef>
#include "Timeline.h"

// singleton TimeHandler instance
Timeline* Timeline::instancePtr = nullptr;


// Singleton Time class
Timeline::Timeline()
    {}

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
