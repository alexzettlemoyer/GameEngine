#include "EventHandler.h"
#include "../GameRunner/ServerGameState.h"
#include <iostream>

static const float CHARACTER_DEATH_TIME = 2.f; //seconds

EventHandler* EventHandler::instancePtr = nullptr;

EventHandler::EventHandler(Timeline* t)
{ 
    timeline = t;
}

EventHandler* EventHandler::getInstance(Timeline* t)
{
    if (instancePtr == NULL)
        { instancePtr = new EventHandler(t); }
    return instancePtr;
}

void EventHandler::onEvent(std::shared_ptr<Event> e)
{
    // std::cout << "Registered event " << e -> metaData << std::endl;
    Event::Variant timeStampVariant = e -> parameters[ Event::Variant::TYPE_TIME_STAMP ];

    std::lock_guard<std::mutex> lock(eventMutex);
    queue[ timeStampVariant.timeStamp ] = e;
}

// character input requires 2 parameters
// timestamp
// character reference
void EventHandler::handleCharacterInput(Event::EventType eventType, std::shared_ptr<Event> e)
{
    Event::Variant characterVariant = e -> parameters[ Event::Variant::TYPE_CHAR_REF ];
    Character* charRef = characterVariant.characterRef;
    switch ( eventType )
    {
        case Event::C_UP:           // character up
            charRef -> up();
            break;
        case Event::C_DOWN:         // character down
            charRef -> down();
            break;
        case Event::C_LEFT:         // character left
            charRef -> left();
            break;
        case Event::C_RIGHT:        // character up
            charRef -> right();
            break;
    }
}

void EventHandler::handleCharacterDeath(std::shared_ptr<Event> e)    // handle character death
{
    Event::Variant characterVariant = e -> parameters[ Event::Variant::TYPE_CHAR_REF ];
    Character* charRef = characterVariant.characterRef;

    // move the character way off screen to imitate death
    charRef -> setPosition(sf::Vector2f(-10000000.f, -10000000.f));

    // now set a new event for the character to respawn 2 seconds in the future
    std::shared_ptr<Event> respawnEvent = std::make_shared<Event>(Event::C_RESPAWN, timeline -> getTimeStamp() + CHARACTER_DEATH_TIME);
    respawnEvent -> addCharacterVariant(charRef);
    onEvent(respawnEvent);
}

void EventHandler::handleCharacterRespawn(std::shared_ptr<Event> e)
{
    Event::Variant characterVariant = e -> parameters[ Event::Variant::TYPE_CHAR_REF ];
    Character* charRef = characterVariant.characterRef;

    charRef -> respawn();
}

void EventHandler::handleTripleUp(std::shared_ptr<Event> e)
{
    if (scriptManager == nullptr)
    {
        std::cout << "Error: No script manager to run script." << std::endl;
        return;
    }
    scriptManager -> runOne("handle_triple_up_event", true, "object_context");
}

void EventHandler::handleCharacterSpawn(std::shared_ptr<Event> e)
{
    Event::Variant characterIdVariant = e -> parameters[ Event::Variant::TYPE_CHAR_ID ];
    int id = characterIdVariant.characterId;

    std::shared_ptr<SpawnPoint> sp = std::make_shared<SpawnPoint>();
    ServerGameState::getInstance() -> addCharacter( std::make_shared<Character>(sp -> getPosition(), id, nullptr, sp) );
}

void EventHandler::handleTicSizeChange(std::shared_ptr<Event> e)
{
    Event::Variant timelineVariant = e -> parameters[ Event::Variant::TYPE_TIMELINE ];
    Event::Variant ticScaleVariant = e -> parameters[ Event::Variant::TYPE_TIC_SCALE ];

    Timeline *timeline = timelineVariant.timeline;
    double newTicScale = ticScaleVariant.ticScale;

    timeline -> editTicSize( newTicScale );
}

void EventHandler::handlePause(std::shared_ptr<Event> e)
{
    Event::Variant timelineVariant = e -> parameters[ Event::Variant::TYPE_TIMELINE ];
    Timeline *timeline = timelineVariant.timeline;

    timeline -> pause();
}

void EventHandler::handleClientDisconnect(std::shared_ptr<Event> e)
{
    Event::Variant characterIdVariant = e -> parameters[ Event::Variant::TYPE_CHAR_ID ];
    int characterId = characterIdVariant.characterId;

    ServerGameState::getInstance() -> removeObject( characterId );
}

void EventHandler::handleCollision(std::shared_ptr<Event> e)
{
    Event::Variant graphicsObjVariant = e -> parameters[ Event::Variant::TYPE_GRAPHICS_OBJ ];
    GraphicsObject* obj = graphicsObjVariant.graphicsObject;

    Event::Variant collisionDirectionVariant = e -> parameters[ Event::Variant::TYPE_COLLISION_DIR ];
    int collisionDirection = collisionDirectionVariant.collisionDirection;

        // x collision 
    if ( collisionDirection == 0 )
    {
        if ( obj -> collisionTypeX == GraphicsObject::ERASE )
            ServerGameState::getInstance() -> removeObject(obj -> identifier());
    }
        // y collision
    else if ( collisionDirection == 1 )
    { }
}

void EventHandler::processEvent(std::shared_ptr<Event> e)
{
    Event::EventType eventType = e -> eventType;

    if ( eventType == Event::C_UP || eventType == Event::C_DOWN || eventType == Event::C_LEFT || eventType == Event::C_RIGHT )
        handleCharacterInput( eventType, e );

    switch ( eventType )
    {
        case Event::C_DEATH:
            handleCharacterDeath(e);
            break;
        case Event::C_RESPAWN:
            handleCharacterRespawn(e);
            break;
        case Event::C_SPAWN:
            handleCharacterSpawn(e);
            break;
        case Event::PAUSE:
            handlePause(e);
            break;
        case Event::TIC_CHANGE:
            handleTicSizeChange(e);
            break;
        case Event::CLIENT_DISCONNECT:
            handleClientDisconnect(e);
            break;
        case Event::COLLISION:
            handleCollision(e);
            break;
        case Event::TRIPLE_UP:
            handleTripleUp(e);
            break;
        default:
            break;
    }
}

void EventHandler::handleEvents()
{
    float currentTime = timeline -> getTimeStamp();
    std::vector<float> handledEvents;

    for ( const auto& event : queue )
    {
        // if the event has already occurred (time has passsed)
        if ( event.first <= currentTime )
        {
            // process it!
            processEvent( event.second );
            std::lock_guard<std::mutex> lock(eventMutex);
            handledEvents.push_back( event.first );
        }
    }

    for ( const auto &i : handledEvents )
    {
        std::lock_guard<std::mutex> lock(eventMutex);
        queue.erase( i );
    }
}

void EventHandler::addScriptManager(std::shared_ptr<ScriptManager> sm)
{
    scriptManager = sm;
}

v8::Local<v8::Object> EventHandler::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
	return v8helpers::exposeToV8("eventhandler", this, v, isolate, context, context_name);
}

std::shared_ptr<Event> sharedPtrFromV8Obj(v8::Local<v8::Object> v8Object) {
    v8::Local<v8::External> wrap2 = v8::Local<v8::External>::Cast(v8Object->GetInternalField(0));
    Event* eventPtr = static_cast<Event*>(wrap2->Value());
    return std::shared_ptr<Event>(eventPtr);
}

void EventHandler::ScriptedRaiseEvent(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	v8::EscapableHandleScope handle_scope(args.GetIsolate());
	v8::Context::Scope context_scope(context);

    if (args.Length() != 2)
    {
        std::cout << "Failed to raise event" << std::endl;
        return;
    }

    // get the event handler args[0]
    v8::Local<v8::Object> eventHandlerObject = args[0]->ToObject(context).ToLocalChecked();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(eventHandlerObject->GetInternalField(0));
    EventHandler* eventHandlerPtr = static_cast<EventHandler*>(wrap->Value());

    // get the event args[1]
    v8::Local<v8::Object> eventObject = args[1]->ToObject(context).ToLocalChecked();
    std::shared_ptr<Event> shared_event_ptr = sharedPtrFromV8Obj(eventObject);

    // raise the event!
    eventHandlerPtr -> onEvent(shared_event_ptr);
}

