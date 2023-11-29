#include <SFML/Graphics.hpp>
#include "ServerGameState.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../Movement/Mover.hpp"
#include "../Events/Event.h"
#include "../Events/EventHandler.h"
#include <sstream>
#include <iostream>

ServerGameState* ServerGameState::instancePtr = nullptr;
enum InputType { HALF, REAL, DOUBLE, PAUSE, CLOSE, MODIFY };

ServerGameState::ServerGameState()
{ 
    setupServerGameState();
}

ServerGameState* ServerGameState::getInstance()
{
    if (instancePtr == NULL)
        { instancePtr = new ServerGameState(); }
    return instancePtr;
}

void ServerGameState::setupServerGameState()
{
    eventHandler = EventHandler::getInstance( timeline.get() );
    findObjById(2) -> setMovementFunction( &movementClockwise );
    findObjById(1) -> setMovementFunction( &movementLeftRight );
    findObjById(4) -> setMovementFunction( &movementUpDown );
}

/**
 * updates the game state
 *  - first update delta time
 *  - continue moving the item clockwise
 *  - continue moving the platform right
 *  - update all character movements
 *  
 * this function is called by the server during each pub-sub loop
 */
void ServerGameState::updateGameState()
{
    EventHandler::getInstance() -> handleEvents();

    timeline -> updateDeltaTime();

    // update movements for all objects
    for ( std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects() )
    {
            // if the object has a movement function ( moves in a pattern ), call it
        if ( i -> getMovementFunction() != nullptr )
            i -> getMovementFunction()(*(i.get()));
    }
}

void ServerGameState::input(std::string objId, std::string i)
{
    int in = stoi( i );

    // if the input is tic size HALF, REAL, DOUBLE
    if ( in == HALF || in == REAL || in == DOUBLE )
    {
        std::shared_ptr<Event> e = std::make_shared<Event>(Event::TIC_CHANGE, timeline -> getTimeStamp());
        e -> addTimelineVariant(timeline.get());

        if ( in == HALF )
            e -> addTicScaleVariant(Timeline::SCALE_HALF);
        if ( in == REAL )
            e -> addTicScaleVariant(Timeline::SCALE_REAL);
        if ( in == DOUBLE )
            e -> addTicScaleVariant(Timeline::SCALE_DOUBLE);

        e -> addMetaData("ServerGameState, from client input " + i);
        EventHandler::getInstance() -> onEvent(e);
    }
    else if ( in == PAUSE )         // timeline paused
    {
        std::shared_ptr<Event> e = std::make_shared<Event>(Event::PAUSE, timeline -> getTimeStamp());
        e -> addTimelineVariant(timeline.get());

        e -> addMetaData("ServerGameState, from client input " + i);
        EventHandler::getInstance() -> onEvent(e);
    }
    else if ( in == CLOSE )         // client window closed -> removes their character
    {
        std::shared_ptr<Event> e = std::make_shared<Event>(Event::CLIENT_DISCONNECT, timeline -> getTimeStamp());
        e -> addCharacterIdVariant(stoi(objId));

        e -> addMetaData("ServerGameState, from client input " + i);
        EventHandler::getInstance() -> onEvent(e);
    }
}

void ServerGameState::updateCharacterPosition(std::string charId, float x, float y )
{
    std::lock_guard<std::mutex> lock(stateMutex);
    std::shared_ptr<GraphicsObject> character = findObjById( stoi(charId) );
    character -> setPosition(sf::Vector2f(x, y));
}

void ServerGameState::addCharacter(std::shared_ptr<Character> newCharacter)
{
    graphicsObjects.push_back( newCharacter );
}

/**
 * creates a new character and adds it to the list of graphics objects
 * assigns the new character with the id of the client which connected to it
 */
int ServerGameState::newCharacter()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    int id = objectId++;

    std::shared_ptr<Event> e = std::make_shared<Event>(Event::C_SPAWN, timeline -> getTimeStamp());
    e -> addCharacterIdVariant( id );

    e -> addMetaData("ServerGameState, new client connected " + id );
    EventHandler::getInstance() -> onEvent(e);
    return id;
}


/**
 * serialize is used by the server
 * to compress relevant data about graphics objects needed by clients
 * format is:
 *      [ dt ][ graphicsObjects ... ]
 *      each graphicsObject is contained in []
 *      [ id position.x position.y velocity.x velocity.y type ]
 *      where type is either
 *          1: character
 *          2: platform
 *          3: item
 */
std::string ServerGameState::serialize()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    std::stringstream s;

    s << "[ " << timeline -> getDt() << " " << timeline -> getTicSize() << " ]";

        // add all graphics objects
    for (std::shared_ptr<GraphicsObject> const& i : graphicsObjects)
    {
        s << "[ " << i -> identifier() << " " << i -> getPosition().x << " " << i -> getPosition().y;
        s << " " << i -> getVelocity().x << " " << i -> getVelocity().y;
        s << " " << i -> getType() << " ]";
    }

        // convert the buffer into a string
    return s.str();
}

