#include <SFML/Graphics.hpp>
#include "Draw/Draw.hpp"
#include "io/ioHandler.h"
#include "Time/Timeline.h"
#include "Time/Thread.cpp"
#include <functional>
#include <iostream>
#include <atomic>

std::atomic<bool> isGameRunning(true);
std::mutex mutex;
std::condition_variable conditionV;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default);
    window.setFramerateLimit(120);

    Draw *draw = Draw::getInstance();
    draw -> setupGraphics(&window);

    ioHandler *io = ioHandler::getInstance(draw -> character);

    // std::thread thread1(&ioHandler::handle, io, std::ref(isGameRunning));
    // std::thread thread2(&Draw::startMovements, draw, std::ref(isGameRunning));
    // {
    //     std::unique_lock<std::mutex> lock(mutex);
    //     conditionV.wait(lock, [] { return !isGameRunning.load(); });
    // }

    while (window.isOpen())
    {
        Timeline::getInstance() -> updateDeltaTime();
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                isGameRunning = false;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Key::P:
                        Timeline::getInstance() -> pause();
                        std::cout << "PAUSE/UNPAUSE" << std::endl;
                        break;
                    case sf::Keyboard::Key::Num1:
                        Timeline::getInstance() -> changeScale(Timeline::SCALE_HALF);
                        std::cout << "0.5" << std::endl;
                        break;
                    case sf::Keyboard::Key::Num2:
                        Timeline::getInstance() -> changeScale(Timeline::SCALE_REAL);
                        std::cout << "1.0" << std::endl;
                        break;
                    case sf::Keyboard::Key::Num3:
                        Timeline::getInstance() -> changeScale(Timeline::SCALE_DOUBLE);
                        std::cout << "2.0" << std::endl;
                        break;
                }
            }
        }
            // *** Keep draw -> drawGraphics(&window) in main thread
            // *** SFML does not support rendering through a thread
        draw -> drawGraphics(&window);

        // io -> handle();
        // draw -> startMovements();

        // std::thread thread1(&ioHandler::handle, io, );
        // std::thread thread2(&Draw::startMovements, draw);

        std::thread thread1(&ioHandler::handle, io, std::ref(isGameRunning));
        std::thread thread2(&Draw::startMovements, draw, std::ref(isGameRunning));

        if (thread1.joinable())
            thread1.join();
        if (thread2.joinable())
            thread2.join();
    }

    // thread1.join();
    // thread2.join();


    delete draw;
    delete io;
    return 0;
}