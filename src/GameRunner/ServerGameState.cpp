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
    deathZone = std::make_shared<DeathZone>(sf::Vector2f(0.f, 700.f), objectId++, timeline);
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
    timeline -> updateDeltaTime();

        // the item should move clockwise, id = 2
    GraphicsObject* item = findObjById(2).get();
    if (item != NULL )
        movementClockwise(*item);
    
        // platform2 should move left right, id = 1
    GraphicsObject* platform2 = findObjById(1).get();
    if (platform2 != NULL )
        movementLeftRight(*platform2);

    GraphicsObject* platform3 = findObjById(4).get();
    if (platform3 != NULL)
        movementUpDown(*platform3);

        // update all the character movements
    for (std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects())
    {
        if ( i -> getType() == GraphicsObject::CHARACTER_TYPE )
            dynamic_cast<Character*>(i.get()) -> updateMovement();
    }
}

/**
 *  input function handles input from clients
 *  this function is called by the server, to apply client input to the game state
 *  
 *  parameters:
 *      objId - the id of the object to apply input to (the character/clients' id)
 *      in - the input string ( a number 1-9 denoted by clients' InputType enum )
 */
void ServerGameState::input(std::string objId, std::string in)
{
    int charId = stoi(objId);
    int i = stoi(in);

    switch (i)
    {
        case 0:             // character jump
            dynamic_cast<Character*>(findObjById(charId).get()) -> up();
            break;
        case 1:             // character down
            dynamic_cast<Character*>(findObjById(charId).get()) -> down();
            break;
        case 2:             // character left
            dynamic_cast<Character*>(findObjById(charId).get()) -> left();
            break;
        case 3:             // character right
            dynamic_cast<Character*>(findObjById(charId).get()) -> right();
            break;
        case 4:
            timeline -> editTicSize(Timeline::SCALE_HALF);
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
            removeObject(charId);
            break;
        default:
            // do nothing
            break;
    }
}

/**
 * serialize is used by the server
 * to compress relevant data about graphics objects needed by clients
 * format is:
 *      each graphicsObject is contained in []
 *      [ id position.x position.y type ]
 *      where type is either
 *          1: character
 *          2: platform
 *          3: item
 */
std::string ServerGameState::serialize()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    std::stringstream s;

        // add all graphics objects
    for (std::shared_ptr<GraphicsObject> const& i : graphicsObjects)
        s << "[ " << i -> identifier() << " " << i -> getPosition().x << " " << i -> getPosition().y << " " << i -> getType() << " ]";

        // convert the buffer into a string
    return s.str();
}

std::shared_ptr<DeathZone> ServerGameState::getDeathZone()
{
    return deathZone;
}
