#ifndef EVENT_H
#define EVENT_H

#include "../GraphicsObject/GraphicsObject.h"

class Event
{
    public:
        Event();

        struct Variant
        {
            enum Type
            {
                TYPE_GAME_OBJ,
                TYPE_CHAR_ID,
                TYPE_TIME_STAMP,
            };
            Type type;
            union
            {
                int characterId;
                GraphicsObject* gameObj;
                float timeStamp;
            };
        };
        std::map<Variant::Type, struct Variant> parameters;
        void addTimeVariant(float timeStamp);
        void addGraphicsObjectVariant(GraphicsObject* g);
};

#endif