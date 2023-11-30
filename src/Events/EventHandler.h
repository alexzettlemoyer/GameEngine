#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event.h"
#include <memory>
#include <v8/v8.h>

class EventHandler
{
    private:
        EventHandler(Timeline *t);
        static EventHandler* instancePtr;
        std::map<float, std::shared_ptr<Event>> queue;
        Timeline* timeline;

        std::mutex eventMutex;

        void processEvent(std::shared_ptr<Event> e);

        // functions used on the client side
        void handleCharacterInput(Event::EventType type, std::shared_ptr<Event> e);
        void handleCharacterDeath(std::shared_ptr<Event> e);
        void handleCharacterRespawn(std::shared_ptr<Event> e);

        // functions used on the server side
        void handleCharacterSpawn(std::shared_ptr<Event> e);
        void handleTicSizeChange(std::shared_ptr<Event> e);
        void handleClientDisconnect(std::shared_ptr<Event> e);
        void handlePause(std::shared_ptr<Event> e);
        void handleCollision(std::shared_ptr<Event> e);

    public:
        static EventHandler* getInstance(Timeline* t = NULL);
        void onEvent(std::shared_ptr<Event> e);
        void handleEvents();

        v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");
        static void ScriptedRaiseEvent(const v8::FunctionCallbackInfo<v8::Value>& args);
};

#endif