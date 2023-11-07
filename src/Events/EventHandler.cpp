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
      std::lock_guard<std::mutex> lock(eventMutex);
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
