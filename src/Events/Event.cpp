#include "Event.h"

Event::Event(EventType eventType, float timeStamp)
{ 
    this -> eventType = eventType;
    Variant v;
    v.type = Variant::TYPE_TIME_STAMP;
    v.timeStamp = timeStamp;
    parameters[Variant::TYPE_TIME_STAMP] = v;
}

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

void Event::addCharacterIdVariant(int characterId)
{
    Variant v;
    v.type = Variant::TYPE_CHAR_ID;
    v.characterId = characterId;
    parameters[Variant::TYPE_CHAR_ID] = v;
}

void Event::addGraphicsObjectVariant(GraphicsObject* graphicsObject)
{
    Variant v;
    v.type = Variant::TYPE_GRAPHICS_OBJ;
    v.graphicsObject = graphicsObject;
    parameters[Variant::TYPE_GRAPHICS_OBJ] = v;
}

void Event::addCollisionDirectionVariant(int collisionDir)
{
    Variant v;
    v.type = Variant::TYPE_COLLISION_DIR;
    v.collisionDirection = collisionDir;
    parameters[Variant::TYPE_COLLISION_DIR] = v;    
}

void Event::addTimelineVariant(Timeline* timelineRef)
{
    Variant v;
    v.type = Variant::TYPE_TIMELINE;
    v.timeline = timelineRef;
    parameters[Variant::TYPE_TIMELINE] = v;
}

void Event::addTicScaleVariant(double newTicScale)
{
    Variant v;
    v.type = Variant::TYPE_TIC_SCALE;
    v.ticScale = newTicScale;
    parameters[Variant::TYPE_TIC_SCALE] = v;
}