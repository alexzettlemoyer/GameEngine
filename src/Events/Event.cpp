#include "Event.h"

Event::Event()
{ }

void Event::addTimeVariant(float timeStamp)
{
    Variant v;
    v.type = Variant::TYPE_TIME_STAMP;
    v.timeStamp = timeStamp;
    parameters[Variant::TYPE_TIME_STAMP] = v;   
}