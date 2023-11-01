#ifndef EVENT_H
#define EVENT_H

#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"

class Event
{
    public:
        Event();

        enum EventType { C_UP, C_DOWN, C_LEFT, C_RIGHT, C_DEATH, C_SPAWN, PAUSE, TIC_CHANGE };
        struct Variant
        {
            enum Type
            {
                TYPE_GAME_OBJ,
                TYPE_CHAR_REF,
                TYPE_TIME_STAMP,
            };
            Type type;
            union
            {
                Character* characterRef;
                GraphicsObject* gameObj;
                float timeStamp;
            };
        };

        EventType eventType;
        std::map<Variant::Type, struct Variant> parameters;
        void setEventType(EventType t);
        void addTimeVariant(float timeStamp);
        void addCharacterVariant(Character* charRef);
        void addGraphicsObjectVariant(GraphicsObject* g);
};

#endif