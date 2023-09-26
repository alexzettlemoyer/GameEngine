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


GameState* GameState::instancePtr = nullptr;

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

void GameState::setupGameState()
{
    // setup graphics objects: platforms and item
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(25.f, 520.f), 1));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(525.f, 650.f), 2));
    graphicsObjects.push_back(std::make_shared<Item>(sf::Vector2f(100.2f, 64.6f), sf::Vector2f(800.f, 150.f), 3));
}

void GameState::updateGameState()
{
    Timeline::getInstance() -> updateDeltaTime();

        // the item should move clockwise, id = 3
    movementClockwise(*findObjById(3));
        // platform2 should move left right, id = 2
    movementLeftRight(*findObjById(2));
}

void GameState::input(std::string in)
{
    int i = stoi(in);
    switch (i)
    {
        case 0:
            // character.jump
            break;
        case 1:
            // character down
            break;
        case 2:
            // character left
            break;
        case 3:
            // character right
            break;
        case 4:
            Timeline::getInstance() -> changeScale(Timeline::SCALE_HALF);
            break;
        case 5:
            Timeline::getInstance() -> changeScale(Timeline::SCALE_REAL);
            break;
        case 6:
            Timeline::getInstance() -> changeScale(Timeline::SCALE_DOUBLE);
            break;
        case 7:
            Timeline::getInstance() -> pause();
            std::cout << "PAUSING" << std::endl;
            break;
        default:
            // do nothing
            break;
    }
}

std::string GameState::serialize()
{
    std::stringstream s;

        // add all graphics objects
    for (std::shared_ptr<GraphicsObject> const& i : graphicsObjects)
        s << "[ " << i -> identifier() << " " << i -> getPosition().x << " " << i -> getPosition().y << " ]";

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

void GameState::deserialize(std::string data)
{
    std::vector<std::string> objs = split(data, ']');
    for ( std::string const& obj : objs )
    {
        std::vector<std::string> objData = split(obj, ' ');

        // ignore objData[0] -> '['
        int id = stoi(objData[1]);
        std::shared_ptr<GraphicsObject> currentObj = findObjById( id );
        if ( currentObj == NULL )
            std::cout << "Error in deserialization: no object with id " << id << std::endl;

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


// std::shared_ptr<Character> GameState::newCharacter()
// {
//         // fix ID num
//     int id = 0;
//     std::shared_ptr<Character> newChar = std::make_shared<Character>(sf::Vector2f(100.f, 180.f), id);
//     characters.push_back(newChar);
//     return newChar;
// }