#include <SFML/Graphics.hpp>
#include "ServerGameState.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../Movement/Mover.hpp"
#include <sstream>
#include <iostream>

ServerGameState* ServerGameState::instancePtr = nullptr;

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
    timeline -> getTimeStamp();

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
    switch( in )    
    {
        case 4:
            timeline -> editTicSize( Timeline::SCALE_HALF );
            std::cout << "HALF" << std::endl;
            break;
        case 5:
            timeline -> editTicSize(Timeline::SCALE_REAL);
            std::cout << "REAL" << std::endl;
            break;
        case 6:
            timeline -> editTicSize(Timeline::SCALE_DOUBLE);
            std::cout << "DOUBLE" << std::endl;
            break;
        case 7:
            timeline -> pause();
            std::cout << (timeline->isPaused() ? "PAUSING" : "UNPAUSING") << std::endl;
            break;
        case 8:     // CLIENT WINDOW CLOSED -> remove their character
            removeObject(stoi( objId ));
            break;
    }
}

void ServerGameState::updateCharacterPosition(std::string charId, float x, float y )
{
    std::lock_guard<std::mutex> lock(stateMutex);
    std::shared_ptr<GraphicsObject> character = findObjById( stoi(charId) );
    character -> setPosition(sf::Vector2f(x, y));
}


/**
 * creates a new character and adds it to the list of graphics objects
 * assigns the new character with the id of the client which connected to it
 */
int ServerGameState::newCharacter()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    int id = objectId++;
    // SpawnPoint *sp = new SpawnPoint();

    std::shared_ptr<SpawnPoint> sp = std::make_shared<SpawnPoint>();
    graphicsObjects.push_back(std::make_shared<Character>(sp -> getPosition(), id, timeline, sp)); // 100, 180
    // delete sp;
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

