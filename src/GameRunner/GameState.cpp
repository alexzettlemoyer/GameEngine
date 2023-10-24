#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <set>
#include "GameState.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/SideBoundary.h"
#include "../GraphicsObject/SpawnPoint.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"
#include "../Time/Timeline.h"
#include "../Movement/Mover.hpp"
#include "../Movement/Collider.hpp"
#include "../Movement/SideScroller.h"

std::mutex stateMutex;
GameState* GameState::instancePtr = nullptr;
Timeline* timeline;

int objectId = 0;

/**
 * Singleton game state instance
 * on instantiation, sets up the initial graphics objects using setupGameState
 */
GameState::GameState()
{
    setupGameState();
}

GameState* GameState::getInstance()
{
    if (instancePtr == NULL)
        { instancePtr = new GameState(); }
    return instancePtr;
}

/**
 * sets up the initial graphicsObjects
 * 2 platforms, and 1 item
 * the setup is called on the server side
 */
void GameState::setupGameState()
{
    std::lock_guard<std::mutex> lock(stateMutex);

    timeline = new Timeline();

    deathZone = std::make_shared<DeathZone>(sf::Vector2f(0.f, 700.f), objectId++, timeline);
    sideBoundaries.push_back(std::make_shared<SideBoundary>(sf::Vector2f(750.f, 0.f), objectId++, timeline, SideBoundary::RIGHT));
    sideBoundaries.push_back(std::make_shared<SideBoundary>(sf::Vector2f(0.f, 0.f), objectId++, timeline, SideBoundary::LEFT));

    // setup graphics objects: platforms and item
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(25.f, 520.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(525.f, 650.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Item>(sf::Vector2f(800.f, 150.f), objectId++, timeline));

    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(1000.f, 500.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(1600.f, 400.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(2200.f, 500.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(2700.f, 500.f), objectId++, timeline));

    std::cout << "successfully added " << graphicsObjects.size() << " Graphics Objects..." << std::endl;
    std::cout << "successfully added " << sideBoundaries.size() << " Side Boundaries..." << std::endl;

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
void GameState::updateGameState()
{
    timeline -> updateDeltaTime();

        // the item should move clockwise, id = 4
    GraphicsObject* item = findObjById(5).get();
    if (item != NULL )
        movementClockwise(*item);
    
        // platform2 should move left right, id = 3
    GraphicsObject* platform2 = findObjById(4).get();
    if (platform2 != NULL )
        movementLeftRight(*platform2);

    GraphicsObject* platform3 = findObjById(7).get();
    if (platform3 != NULL)
        movementUpDown(*platform3);

        // update all the character movements
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects())
        {
        if ( i -> getType() == GraphicsObject::CHARACTER_TYPE )
         {   dynamic_cast<Character*>(i.get()) -> updateMovement();}
        std::cout << i -> identifier() << ": " << i -> getPosition().x << " " << i -> getPosition().y << std::endl;
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
void GameState::input(std::string objId, std::string in)
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
std::string GameState::serialize()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    std::stringstream s;

        // add all graphics objects
    for (std::shared_ptr<GraphicsObject> const& i : graphicsObjects)
        s << "[ " << i -> identifier() << " " << i -> getPosition().x << " " << i -> getPosition().y << " " << i -> getType() << " ]";

        // convert the buffer into a string
    return s.str();
}

std::vector<std::string> GameState::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream stream(str);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * deserialize is used by clients
 * to process the graphics object data sent by the client
 * format is:
 *      each graphicsObject is contained in []
 *      [ id position.x position.y type ]
 * sets up the GameState using the data
 * add any unrecognized objects
 */
void GameState::deserialize(std::string data)
{
    std::vector<std::string> objs = split(data, ']');
    std::set<int> currentGameIds;

    for ( std::string const& obj : objs )
    {
        std::vector<std::string> objData = split(obj, ' ');

        // ignore objData[0] -> '['
        int id = stoi(objData[1]);
        currentGameIds.insert(id);
        std::shared_ptr<GraphicsObject> currentObj = findObjById( id );

        // there is a new graphics object that this client hasn't stored yet
        if ( currentObj == NULL )
        {
            switch(stoi(objData[4]))
            {
                case GraphicsObject::CHARACTER_TYPE:
                    graphicsObjects.push_back(std::make_shared<Character>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id, timeline));
                    break;
                case GraphicsObject::PLATFORM_TYPE:
                    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id, timeline));
                    break;
                case GraphicsObject::ITEM_TYPE:
                    graphicsObjects.push_back(std::make_shared<Item>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id, timeline));
                    break;
                default:
                    std::cout << "Error in deserialization: object cannot be created. id: " << id << std::endl;
                    break;
            }
        }
        else
        {
            currentObj.get() -> setPosition( sf::Vector2f(stof(objData[2]), stof(objData[3])) );
            // if ( data.size() > 4 )
            // {
            //     data[ 5 ] // extra message
            // }
        }
    }

        // compare the ids given by the server to the current ids we have in our graphics object list
    if ( currentGameIds.size() != graphicsObjects.size() )
    {
        for ( std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects() )
        {
            // found the object we need to remove
            if ( currentGameIds.find(i->identifier()) == currentGameIds.end())
                removeObject(i -> identifier());
        }
    }
}

void GameState::checkSideCollision(int characterId)
{
    std::shared_ptr<GraphicsObject> thisCharacter = findObjById( characterId );
    if ( thisCharacter == NULL )
        return;
    for (std::shared_ptr<SideBoundary> const& i : getSideBoundaries())
    {
        if (SideScroller::getInstance() -> checkSideCollision(thisCharacter.get(), i.get()))
            break;
    }
    scrollObjects();
}

void GameState::scrollObjects()
{
    float totalScrollDistance = SideScroller::getInstance() -> getSideScrollDistance();
    for (std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects()) 
    {
        sf::Vector2f currentCoords = i -> getPosition();
        i -> setPosition(currentCoords - sf::Vector2f( totalScrollDistance, 0.f ));
    }


}

std::shared_ptr<GraphicsObject> GameState::findObjById(int id)
{
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects())
    {
        if ( i -> identifier() == id )
            return i;
    }
    return NULL;
}

std::list<std::shared_ptr<GraphicsObject>> GameState::getGraphicsObjects()
{
    return graphicsObjects;
}

std::shared_ptr<DeathZone> GameState::getDeathZone()
{
    return deathZone;
}

std::list<std::shared_ptr<SideBoundary>> GameState::getSideBoundaries()
{
    return sideBoundaries;
}

/**
 * creates a new character and adds it to the list of graphics objects
 * assigns the new character with the id of the client which connected to it
 */
int GameState::newCharacter()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    int id = objectId++;
    SpawnPoint *sp = new SpawnPoint();
    graphicsObjects.push_back(std::make_shared<Character>(sp -> getPosition(), id, timeline, sp)); // 100, 180
    delete sp;
    return id;
}

// void GameState::respawn( int id )
// {
//     std::lock_guard<std::mutex> lock(stateMutex);
//     SpawnPoint *sp = new
//     findObjById( id )
// }

// void Character::respawn()
// {
//     this -> spawnPoint = new SpawnPoint();
//     // SideScroller::getInstance() -> reset();
//     distanceTravelled = 0.f;
//     this -> setPosition(spawnPoint -> getPosition());
//     this -> velocity = sf::Vector2f(0.f, 0.f);
// }

void GameState::removeObject(int id)
{
    std::lock_guard<std::mutex> lock(stateMutex);
    graphicsObjects.remove(findObjById(id));
}

std::list<Character*> GameState::getCharacters()
{
    std::list<Character*> characters;
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects())
        if ( i -> getType() == GraphicsObject::CHARACTER_TYPE )
            characters.push_back((dynamic_cast<Character*>((i.get()))));
    return characters;
}
