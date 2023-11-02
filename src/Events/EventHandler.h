#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event.h"
#include <memory>

class EventHandler
{
    private:
        EventHandler(Timeline *t);
        static EventHandler* instancePtr;
        std::map<float, std::shared_ptr<Event>> queue;
        Timeline* timeline;

        void processEvent(std::shared_ptr<Event> e);

        // functions used on the client side
        void handleCharacterInput(Event::EventType type, std::shared_ptr<Event> e);
        void handleCharacterDeath(std::shared_ptr<Event> e);
        void handleCharacterRespawn(std::shared_ptr<Event> e);

        // functions used on the server side
        void handleCharacterSpawn(std::shared_ptr<Event> e);
        void handleTicSizeChange(std::shared_ptr<Event> e);
        void handleWindowClose(std::shared_ptr<Event> e);
        void handlePause(std::shared_ptr<Event> e);
        void handleCollision(std::shared_ptr<Event> e);

    public:
        static EventHandler* getInstance(Timeline* t = NULL);
        void onEvent(std::shared_ptr<Event> e);
        void handleEvents();
};

#endif