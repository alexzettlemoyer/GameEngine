#include "Event.h"

Event::Event()
{ }

void Event::setEventType(EventType t)
{
    this -> eventType = t;
}

void Event::addTimeVariant(float timeStamp)
{
    Variant v;
    v.type = Variant::TYPE_TIME_STAMP;
    v.timeStamp = timeStamp;
    parameters[Variant::TYPE_TIME_STAMP] = v;   
}

void Event::addCharacterVariant(Character* charRef)
{
    Variant v;
    v.type = Variant::TYPE_CHAR_REF;
    v.characterRef = charRef;
    parameters[Variant::TYPE_CHAR_REF] = v;
}