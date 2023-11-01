#include <SFML/Graphics.hpp>
#include "GameRunner.h"
#include "../Movement/Collider.hpp"
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
    this -> characterId = clientId;
    window.setFramerateLimit(100);
    window.setView(view);

    //     // setup background
    // if (!backgroundTexture.loadFromFile(IMG_BACKGROUND))
    //     {} // Handle error
    // background.setScale(1.f, 1.f);
    // background.setTexture(backgroundTexture);
    background.setFillColor(sf::Color(200, 252, 252));
    background.setSize(sf::Vector2f(3000.f, 800.f));
    background.setPosition(sf::Vector2f(0.f, 0.f));

    game = ClientGameState::getInstance( characterId );
}

GameRunner* GameRunner::getInstance(int id)
{
    if ( instancePtr == NULL )
        { instancePtr = new GameRunner(id); }
    return instancePtr;
}

GameRunner::~GameRunner()
{
    delete game;
}

int GameRunner::getCharacterId()
{
    return characterId;
}

/**
 * deserializes game state data using the gameState deserialize function
 * used by the client to deserialize game state data (object positions)
 */
void GameRunner::deserialize(std::string data)
{
    game -> deserialize( data, characterId );
}

void GameRunner::adjustView()
{
    std::shared_ptr<Character> character = game -> getCharacter();
    sf::Vector2f characterPosition = character -> getPosition();

    if ( characterPosition.x < 500.f )
        characterPosition.x = 500.f;
    if ( characterPosition.x > 2500.f )
        characterPosition.x = 2500.f;

    view.setCenter(characterPosition.x - 2.f, 400);
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

    adjustView();
    window.setView(view);
    // for ( std::shared_ptr<SideBoundary> const& i : ClientGameState::getInstance() -> getSideBoundaries() )
    //     window.draw(*i);
    window.display();
}

sf::RenderWindow *GameRunner::getWindow()
{
    return &window;
}
