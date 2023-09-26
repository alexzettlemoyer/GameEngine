#include <SFML/Graphics.hpp>
#include "GameRunner.h"
#include "GameState.h"
#include "../Draw/Draw.hpp"
#include "../io/ioHandler.h"
#include "../Time/Timeline.h"
#include "../Time/Thread.cpp"
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


// Singleton GameRunner class
GameRunner::GameRunner()
    : window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default)
{
    window.setFramerateLimit(120);

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



void GameRunner::drawGraphics()
{
    // GameState *state = GameState::getInstance();
    window.draw(background);
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects()) {
        window.draw(*i);
    }
    for (std::shared_ptr<Character> const & i : GameState::getInstance() -> getCharacters()) {
        window.draw(*i);
    }
    window.display();
}

sf::RenderWindow *GameRunner::getWindow()
{
    return &window;
}



// sf::RenderWindow *GameRunner::gameLoop()
// {
//     Timeline::getInstance() -> updateDeltaTime();
    
//     sf::Event event;
//     while (this.window.pollEvent(event))
//     {
//         if (event.type == sf::Event::Closed)
//         {
//             window -> close();
//             isGameRunning = false;
//             return NULL;
//         }
//         else if (event.type == sf::Event::KeyPressed)
//         {
//             switch (event.key.code)
//             {
//                 case sf::Keyboard::Key::P:
//                     Timeline::getInstance() -> pause();
//                     std::cout << "PAUSE/UNPAUSE" << std::endl;
//                     break;
//                 case sf::Keyboard::Key::Num1:
//                     Timeline::getInstance() -> changeScale(Timeline::SCALE_HALF);
//                     std::cout << "0.5" << std::endl;
//                     break;
//                 case sf::Keyboard::Key::Num2:
//                     Timeline::getInstance() -> changeScale(Timeline::SCALE_REAL);
//                     std::cout << "1.0" << std::endl;
//                     break;
//                 case sf::Keyboard::Key::Num3:
//                     Timeline::getInstance() -> changeScale(Timeline::SCALE_DOUBLE);
//                     std::cout << "2.0" << std::endl;
//                     break;
//             }
//         }
//     }
//         // *** Keep draw -> drawGraphics(&window) in main thread
//         // *** SFML does not support rendering through a thread
//     Draw::getInstance() -> drawGraphics(window);

//     std::thread thread1(&ioHandler::handle, ioHandler::getInstance(), std::ref(isGameRunning));
//     std::thread thread2(&Draw::startMovements, Draw::getInstance(), std::ref(isGameRunning));

//     if (thread1.joinable())
//         thread1.join();
//     if (thread2.joinable())
//         thread2.join();

//     return window;
// }

sf::RenderWindow *startGame()
{
    return NULL;
}

int runMain()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default);
    window.setFramerateLimit(120);

    Draw *draw = Draw::getInstance();
    draw -> setupGraphics(&window);

    ioHandler *io = ioHandler::getInstance(draw -> character);

    // while (window.isOpen())
    //     gameLoop(&window, draw, io);

    delete draw;
    delete io;
    return 0;
}