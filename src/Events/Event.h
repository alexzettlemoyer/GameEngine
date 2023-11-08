#ifndef EVENT_H
#define EVENT_H

#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include <list>

class Event
{
    public:
        // Event();

        enum EventType { C_UP, C_DOWN, C_LEFT, C_RIGHT, C_DEATH, C_RESPAWN, C_SPAWN, COLLISION, PAUSE, TIC_CHANGE, CLIENT_DISCONNECT };
        struct Variant
        {
            enum Type
            {
                TYPE_GRAPHICS_OBJ,
                TYPE_CHAR_REF,
                TYPE_TIME_STAMP,
                TYPE_TIMELINE,
                TYPE_TIC_SCALE,
                TYPE_CHAR_ID,
                TYPE_COLLISION_DIR,
            };
            Type type;
            union
            {
                int collisionDirection;   // 0 = x collision, 1 = y collision
                int characterId;
                double ticScale;
                float timeStamp;
                Character* characterRef;
                GraphicsObject* graphicsObject;
                Timeline* timeline;
            };
        };

        Event(EventType eventType, float timeStamp);

        EventType eventType;
        std::map<Variant::Type, struct Variant> parameters;
        std::string metaData;

        // std::shared_ptr<Timeline> *handlerTimeline;

        // some utility functions
        void setEventType(EventType t);
        void addTimeVariant(float timeStamp);
        void addCharacterVariant(Character* charRef);
        void addCharacterIdVariant(int charId);
        void addTimelineVariant(Timeline* timelineRef);
        void addTicScaleVariant(double newTicScale);
        void addGraphicsObjectVariant(GraphicsObject* g);
        void addCollisionDirectionVariant(int collisionDir);
        void addMetaData(std::string data);
};

#endif