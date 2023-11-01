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

// character input requires 2 parameters
// timestamp
// character reference
void EventHandler::handleCharacterInput(Event::EventType eventType, Event e)
{
    for ( const auto& param : e.parameters )
    {
        if ( param.first == Event::Variant::TYPE_CHAR_REF )
        {
            Character* charRef = param.second.characterRef;
            charRef -> up();
        }
    }
}

void EventHandler::processEvent(Event e)
{
    Event::EventType eventType = e.eventType;

    switch ( eventType )
    {
        case Event::C_UP:          // character up
            handleCharacterInput(Event::C_UP, e);
            break;
        case Event::C_DOWN:
            // character down
            break;
        case Event::C_LEFT:
            // character up
            break;
        case Event::C_RIGHT:
            // character up
            break;
        case Event::C_DEATH:
            // character up
            break;
        case Event::C_SPAWN:
            // character up
            break;
        case Event::PAUSE:
            // character up
            break;
        case Event::TIC_CHANGE:
            // character up
            break;
        default:
            break;
    }

    for ( const auto& param : e.parameters )
    {

    }
}

void EventHandler::handleEvents()
{
    // float currentTime = timeline -> getTimeStamp();
    std::vector<float> handledEvents;


    for ( const auto& event : queue )
    {
        // if the event has already occurred (time has passsed)
        // if ( event.first <= currentTime )
        // {
            // process it!
            processEvent( event.second );
            handledEvents.push_back( event.first );
        // }
    }

    for ( const auto &i : handledEvents )
        queue.erase( i );
}
