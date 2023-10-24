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

    timeline = new Timeline();

    // setup graphics objects: platforms and item
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(25.f, 520.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(525.f, 650.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Item>(sf::Vector2f(800.f, 150.f), objectId++, timeline));

    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(1000.f, 500.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(1600.f, 400.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(2200.f, 500.f), objectId++, timeline));
    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(2700.f, 500.f), objectId++, timeline));

    std::cout << "Successfully added " << graphicsObjects.size() << " Graphics Objects..." << std::endl;
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

void GameState::removeObject(int id)
{
    std::lock_guard<std::mutex> lock(stateMutex);
    graphicsObjects.remove(findObjById(id));
}

std::list<Character*> GameState::getCharacters()
{
    std::list<Character*> characters;
    for (std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects())
        if ( i -> getType() == GraphicsObject::CHARACTER_TYPE )
            characters.push_back((dynamic_cast<Character*>((i.get()))));
    return characters;
}
