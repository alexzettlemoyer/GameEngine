#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include "GameState.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"
#include "../Time/Timeline.h"
#include "../Movement/Mover.hpp"

std::mutex stateMutex;
GameState* GameState::instancePtr = nullptr;

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

    // setup graphics objects: platforms and item
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(25.f, 520.f), 0));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(525.f, 650.f), 1));
    graphicsObjects.push_back(std::make_shared<Item>(sf::Vector2f(800.f, 150.f), 2));

    // Timeline::getInstance() -> changeScale(0.0005);
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
    Timeline::getInstance() -> updateDeltaTime();

        // the item should move clockwise, id = 2
    movementClockwise(*findObjById(2));
        // platform2 should move left right, id = 1
    movementLeftRight(*findObjById(1));

        // update all the character movements
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects())
        if ( i -> getType() == GraphicsObject::CHARACTER_TYPE )
            dynamic_cast<Character*>(i.get()) -> updateMovement();
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
    std::lock_guard<std::mutex> lock(stateMutex);
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
            Timeline::getInstance() -> editTicSize(Timeline::SCALE_HALF);
            break;
        case 5:
            Timeline::getInstance() -> editTicSize(Timeline::SCALE_REAL);
            break;
        case 6:
            Timeline::getInstance() -> editTicSize(Timeline::SCALE_DOUBLE);
            break;
        case 7:
            Timeline::getInstance() -> pause();
            std::cout << "PAUSING" << std::endl;
            break;
        case 8:     // CLIENT WINDOW CLOSED -> remove their character
            removeCharacter(charId);
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
    std::lock_guard<std::mutex> lock(stateMutex);
    std::vector<std::string> objs = split(data, ']');

    // std::cout << data << std::endl;

    for ( std::string const& obj : objs )
    {
        std::vector<std::string> objData = split(obj, ' ');

        // ignore objData[0] -> '['
        int id = stoi(objData[1]);
        std::shared_ptr<GraphicsObject> currentObj = findObjById( id );

        // there is a new graphics object that this client hasn't stored yet
        if ( currentObj == NULL )
        {
            switch(stoi(objData[4]))
            {
                case GraphicsObject::CHARACTER_TYPE:
                    std::cout << "NEW CHARACTER" << std::endl;
                    graphicsObjects.push_back(std::make_shared<Character>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id));
                    break;
                case GraphicsObject::PLATFORM_TYPE:
                    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id));
                    break;
                case GraphicsObject::ITEM_TYPE:
                    graphicsObjects.push_back(std::make_shared<Item>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id));
                    break;
                default:
                    std::cout << "Error in deserialization: object cannot be created. id: " << id << std::endl;
                    break;
            }
        }
        else
            currentObj.get() -> setPosition( sf::Vector2f(stof(objData[2]), stof(objData[3])) );
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

/**
 * creates a new character and adds it to the list of graphics objects
 * assigns the new character with the id of the client which connected to it
 */
int GameState::newCharacter()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    int id = graphicsObjects.size();
    graphicsObjects.push_back(std::make_shared<Character>(sf::Vector2f(100.f, 180.f), id));
    return id;
}

void GameState::removeCharacter(int id)
{
    graphicsObjects.remove(findObjById(id));
}