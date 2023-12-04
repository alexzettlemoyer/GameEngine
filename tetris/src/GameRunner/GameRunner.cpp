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

/**
 * Singleton GameRunner class
 * creates the instance, window, and sets the background
 */
GameRunner::GameRunner(int clientId)
    : window(sf::VideoMode(375, 480), "Tetris", sf::Style::Default)
{
    this -> characterId = clientId;
    window.setFramerateLimit(100);
    window.setKeyRepeatEnabled(false);

    gridBounds.setPosition(15.f, 15.f);
    gridBounds.setSize(sf::Vector2f(250.f, 400.f));
    gridBounds.setOutlineColor(sf::Color::Black);
    gridBounds.setOutlineThickness(2.f);
    gridBounds.setFillColor(sf::Color::Transparent);

    background.setFillColor(sf::Color(200, 252, 252));
    background.setSize(sf::Vector2f(375.f, 480.f));
    background.setPosition(sf::Vector2f(0.f, 0.f));

    if (!font.loadFromFile("fonts/tetris-font.ttf"))
    { 
        std::cout << "Failed to load font" << std::endl;
    }
    lineNumber.setFont(font);
    lineNumber.setCharacterSize(10);
    lineNumber.setFillColor(sf::Color::Black);
    lineNumber.setPosition(290.f, 25.f);

    start.setFont(font);
    start.setCharacterSize(10);
    start.setFillColor(sf::Color::Black);
    start.setPosition(290.f, 390.f);
    start.setString("Start");

    game = ClientGameState::getInstance( characterId );
}

GameRunner* GameRunner::getInstance(int id)
{
    if ( instancePtr == NULL )
        { instancePtr = new GameRunner(id); }
    return instancePtr;
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

/**
 * draws all graphicsObjects on screen and displays the updates
 */
void GameRunner::drawGraphics()
{
    window.clear();
    window.draw(background);

    window.draw(gridBounds);
    window.draw(start);

    // std::cout << ClientGameState::getInstance() -> getLineCount() << std::endl;
    lineNumber.setString( "Lines\n\n" + std::to_string(ClientGameState::getInstance() -> getLineCount()) );
    window.draw(lineNumber);

    std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> grid = ClientGameState::getInstance() -> getGrid();

    for (int i = 0; i < ClientGameState::ROW_COUNT; i++)
    {
        for (int j = 0; j < ClientGameState::COLS; j++)
        {
            if ((*grid)[i][j] != nullptr)
                window.draw( *((*grid)[i][j]));
        }
    }

    window.display();
}

sf::RenderWindow *GameRunner::getWindow()
{
    return &window;
}

sf::Text GameRunner::getStartText()
{
    return start;
}