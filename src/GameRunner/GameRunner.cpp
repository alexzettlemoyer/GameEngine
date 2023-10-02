#include <SFML/Graphics.hpp>
#include "GameRunner.h"
#include "GameState.h"
#include "../Draw/Draw.hpp"
#include "../io/ioHandler.h"
#include "../Time/Timeline.h"
#include "../Time/Thread.cpp"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include <sstream>
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
GameRunner::GameRunner()
    : window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default)
{
    window.setFramerateLimit(100);

        // setup background
    if (!backgroundTexture.loadFromFile(IMG_BACKGROUND))
        {} // Handle error
    background.setScale(1.f, 1.f);
    background.setTexture(backgroundTexture);
}

GameRunner* GameRunner::getInstance()
{
    if ( instancePtr == NULL )
        { instancePtr = new GameRunner(); }
    return instancePtr;
}

/**
 * deserializes game state data using the gameState deserialize function
 * used by the client to deserialize game state data (object positions)
 */
void GameRunner::deserialize(std::string data)
{
    GameState::getInstance() -> deserialize( data );
}

/**
 * draws all graphicsObjects on screen and displays the updates
 */
void GameRunner::drawGraphics()
{
    window.clear();
    window.draw(background);
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects()) {
        window.draw(*i);
    }
    window.display();
}

sf::RenderWindow *GameRunner::getWindow()
{
    return &window;
}
