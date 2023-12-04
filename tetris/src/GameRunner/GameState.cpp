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

// int objectId = 0;
int GameState::objectId = 0;

/**
 * Singleton game state instance
 * on instantiation, sets up the initial graphics objects using setupGameState
 */
GameState::GameState()
{
    setupGameState();
}

/**
 * sets up the initial graphicsObjects
 * 2 platforms, and 1 item
 * the setup is called on the server side
 */
void GameState::setupGameState()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    timeline = std::make_shared<Timeline>();
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

std::shared_ptr<GraphicsObject> GameState::findObjById(int id)
{
    for (std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects())
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

void GameState::removeObject(int id)
{
    std::lock_guard<std::mutex> lock(stateMutex);
    graphicsObjects.remove(findObjById(id));
}

std::list<Character*> GameState::getCharacters()
{
    std::list<Character*> characters;
    std::lock_guard<std::mutex> lock(stateMutex);
    for (std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects())
        if ( i -> getType() == GraphicsObject::CHARACTER_TYPE )
            characters.push_back((dynamic_cast<Character*>((i.get()))));
    return characters;
}
