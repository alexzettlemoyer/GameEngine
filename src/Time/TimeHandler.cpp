#include <SFML/System/Clock.hpp>
#include <cstddef>
#include "TimeHandler.h"

// singleton TimeHandler instance
// TimeHandler TimeHandler::instance;
TimeHandler* TimeHandler::instancePtr = nullptr;


// Singleton Time class
TimeHandler::TimeHandler()
    {}

TimeHandler* TimeHandler::getInstance()
{
    if ( instancePtr == NULL )
    {
        instancePtr = new TimeHandler();
    }
    return instancePtr;
}

void TimeHandler::updateDeltaTime()
{
    dt = dt_clock.restart().asSeconds();
}

sf::Time TimeHandler::elapsedTime()
{
    return dt_clock.getElapsedTime();
}
