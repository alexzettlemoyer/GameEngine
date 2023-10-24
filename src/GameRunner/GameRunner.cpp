#include <SFML/Graphics.hpp>
#include "GameRunner.h"
#include "ClientGameState.h"
#include "../Time/Timeline.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include <functional>
#include <iostream>
#include <memory>
#include <atomic>
#include <list>

GameRunner* GameRunner::instancePtr = nullptr;
std::atomic<bool> isGameRunning(true);

static const std::string IMG_BACKGROUND = "images/background.jpeg";

/**
 * Singleton GameRunner class
 * creates the instance, window, and sets the background
 */
GameRunner::GameRunner(int clientId)
    : window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default)
{
    this -> characterId = clientId = clientId;
    window.setFramerateLimit(100);
    // window.setView(view);

        // setup background
    if (!backgroundTexture.loadFromFile(IMG_BACKGROUND))
        {} // Handle error
    background.setScale(1.f, 1.f);
    background.setTexture(backgroundTexture);

    ClientGameState::getInstance();
}

GameRunner* GameRunner::getInstance(int id)
{
    if ( instancePtr == NULL )
        { instancePtr = new GameRunner(id); }
    return instancePtr;
}

/**
 * deserializes game state data using the gameState deserialize function
 * used by the client to deserialize game state data (object positions)
 */
void GameRunner::deserialize(std::string data)
{
    ClientGameState::getInstance() -> deserialize( data );
}

void GameRunner::checkWindowScroll()
{
    ClientGameState::getInstance() -> checkSideCollision( characterId );
}

/**
 * draws all graphicsObjects on screen and displays the updates
 */
void GameRunner::drawGraphics()
{
    window.clear();
    window.draw(background);
    for (std::shared_ptr<GraphicsObject> const& i : ClientGameState::getInstance() -> getGraphicsObjects()) {
        window.draw(*i);
    }
    window.display();
}

sf::RenderWindow *GameRunner::getWindow()
{
    return &window;
}
