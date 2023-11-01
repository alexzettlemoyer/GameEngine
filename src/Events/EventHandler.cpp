#include "EventHandler.h"
#include <iostream>

EventHandler* EventHandler::instancePtr = nullptr;

EventHandler::EventHandler(Timeline* t)
{ 
    timeline = t;
}

EventHandler* EventHandler::getInstance(Timeline* t)
{
    if (instancePtr == NULL)
        { instancePtr = new EventHandler(t); }
    return instancePtr;
}

void EventHandler::onEvent(Event e)
{
    for ( const auto& param : e.parameters )
    {
        if ( param.first == Event::Variant::TYPE_TIME_STAMP )
            queue[ param.first ] = e;
    }
}

void EventHandler::handleEvents()
{
    float currentTime = timeline -> getTimeStamp();

    for ( const auto& event : queue )
    {
        // if the event has already occurred (time has passsed)
        if ( event.first <= currentTime )
        {
            // process it!
        }
    }
}