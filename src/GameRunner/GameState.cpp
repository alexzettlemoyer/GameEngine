#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include <memory>
#include "GameState.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"



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

}

std::string serialize()
{
    return "";
}
void deserialize(std::string data)
{

}

std::list<std::shared_ptr<GraphicsObject>> GameState::getGraphicsObjects()
{
    return graphicsObjects;
}
std::list<std::shared_ptr<Character>> GameState::getCharacters()
{
    return characters;
}

std::shared_ptr<Character> GameState::newCharacter()
{
        // fix ID num
    int id = 0;
    std::shared_ptr<Character> newChar = std::make_shared<Character>(sf::Vector2f(100.f, 180.f), id);
    characters.push_back(newChar);
    return newChar;
}