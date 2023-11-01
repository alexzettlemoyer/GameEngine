#include <SFML/Graphics.hpp>
#include "ClientGameState.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"
#include "../GraphicsObject/Character.h"
#include "../Events/EventHandler.h"
#include <set>
#include <iostream>

ClientGameState* ClientGameState::instancePtr = nullptr;

ClientGameState::ClientGameState(int id)
{
    setupClientGameState(id);
}

ClientGameState* ClientGameState::getInstance(int id)
{
    if (instancePtr == NULL)
        { instancePtr = new ClientGameState(id); }
    return instancePtr;
}

void ClientGameState::setupClientGameState(int id)
{
    std::lock_guard<std::mutex> lock(stateMutex);

    sideBoundaries.push_back(std::make_shared<SideBoundary>(sf::Vector2f(-500.f, 0.f), objectId++, timeline, SideBoundary::LEFT));
    sideBoundaries.push_back(std::make_shared<SideBoundary>(sf::Vector2f(3000.f, 0.f), objectId++, timeline, SideBoundary::RIGHT));

    deathZone = std::make_shared<DeathZone>(sf::Vector2f(0.f, 700.f), objectId++, timeline);
    dt = 0.01;

    std::shared_ptr<SpawnPoint> sp = std::make_shared<SpawnPoint>();

    graphicsObjects.push_back(std::make_shared<Character>(sp -> getPosition(), id, timeline, sp)); // 100, 180
    thisCharacter = std::dynamic_pointer_cast<Character>(findObjById(id));
}

void ClientGameState::updateGameState()
{
    EventHandler::getInstance() -> handleEvents();
    thisCharacter -> updateMovement();
}

sf::Vector2f ClientGameState::getCharacterPosition()
{
    return sf::Vector2f(thisCharacter -> getPosition());
}

std::shared_ptr<Character> ClientGameState::getCharacter()
{
    return thisCharacter;
}

/**
 *  input function handles input from clients
 *  this function is called by the server, to apply client input to the game state
 *  
 *  parameters:
 *      objId - the id of the object to apply input to (the character/clients' id)
 *      in - the input string ( a number 1-9 denoted by clients' InputType enum )
 */
void ClientGameState::input(std::string objId, int in)
{
    // int charId = stoi(objId);

    switch (in)
    {
        // case 0:             // character jump
        //     thisCharacter -> up();
        //     break;
        case 1:             // character down
            thisCharacter -> down();
            break;
        case 2:             // character left
            thisCharacter -> left();
            break;
        case 3:             // character right
            thisCharacter -> right();
            break;
        default:
            break;  // do nothing
    }
}

/**
 * deserialize is used by clients
 * to process the graphics object data sent by the client
 * format is:
 *      [ dt ][ graphicsObjects ... ]
 *      each graphicsObject is contained in []
 *      [ id position.x position.y velocity.x velocity.y type ]
 * sets up the GameState using the data
 * add any unrecognized objects
 */
void ClientGameState::deserialize(std::string data, int characterId)
{
    std::vector<std::string> objs = split(data, ']');
    std::set<int> currentGameIds;

    // get delta time
    std::vector<std::string> timelineInfo = split(objs[ 0 ], ' ');
    dt = stof(timelineInfo[ 1 ]);
    ticSize = stof(timelineInfo[2]);

    for ( int i = 1; i < objs.size(); i++ )
    {
        std::vector<std::string> objData = split(objs[ i ], ' ');

        int id = stoi(objData[1]);
        {
            std::lock_guard<std::mutex> lock(stateMutex);
            currentGameIds.insert(id);
        }

        std::shared_ptr<GraphicsObject> currentObj = findObjById( id );

        // there is a new graphics object that this client hasn't stored yet
        if ( currentObj == NULL )
        {
            switch(stoi(objData[6]))
            {
                case GraphicsObject::CHARACTER_TYPE:
                    {
                        std::lock_guard<std::mutex> lock(stateMutex);
                        graphicsObjects.push_back(std::make_shared<Character>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id, timeline));
                    }
                    break;
                default:
                    std::cout << "Error in deserialization: object cannot be created. id: " << id << std::endl;
                    break;
            }
        }
        else
        {
            if ( currentObj.get() -> identifier() != characterId )
            {
                sf::Vector2f newCoords = sf::Vector2f(stof(objData[2]), stof(objData[3]));

                std::lock_guard<std::mutex> lock(stateMutex);
                currentObj.get() -> setPosition( newCoords );
                currentObj.get() -> velocity = sf::Vector2f(stof(objData[4]), stof(objData[5]));
            }
        }
    }

        // compare the ids given by the server to the current ids we have in our graphics object list
    if ( currentGameIds.size() != graphicsObjects.size() )
    {
        for ( std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects() )
        {
            // found the object we need to remove
            if ( currentGameIds.find(i->identifier()) == currentGameIds.end())
                removeObject(i -> identifier());
        }
    }
}

std::list<std::shared_ptr<SideBoundary>> ClientGameState::getSideBoundaries()
{
    return sideBoundaries;
}

float ClientGameState::getDt()
{
    return dt;
}

float ClientGameState::getTicSize()
{
    return ticSize;
}

std::shared_ptr<DeathZone> ClientGameState::getDeathZone()
{
    return deathZone;
}