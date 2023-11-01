#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event.h"

class EventHandler
{
    private:
        EventHandler();
        static EventHandler* instancePtr;
        std::map<float, Event e> queue;
        Timeline* timeline;

    public:
        static EventHandler* getInstance(Timeline* t = NULL);
        enum EventType { C_UP, C_DOWN, C_LEFT, C_RIGHT, C_DEATH, C_SPAWN, PAUSE, TIC_CHANGE };
        void onEvent( Event e );
        void handleEvents();
};

#endif