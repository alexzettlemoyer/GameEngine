#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event.h"

class EventHandler
{
    private:
        EventHandler(Timeline *t);
        static EventHandler* instancePtr;
        std::map<float, Event> queue;
        Timeline* timeline;

        void processEvent(Event e);
        void handleCharacterInput(Event::EventType type, Event e);

    public:
        static EventHandler* getInstance(Timeline* t = NULL);
        void onEvent( Event e );
        void handleEvents();
};

#endif