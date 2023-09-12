#include <SFML/Graphics.hpp>
#include "Draw/Draw.hpp"
#include "io/ioHandler.h"
#include "Time/TimeHandler.h"
#include "Time/Thread.cpp"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default);
    window.setFramerateLimit(120);

    Draw *draw = Draw::getInstance();
    draw -> setupGraphics(&window);

    ioHandler *io = new ioHandler(draw -> character);

    // Mutex to handle locking, condition variable to handle notifications between threads
    // std::mutex m;
    // std::condition_variable cv;

    // // Create thread objects
    // Thread t1(0, NULL, &m, &cv);
    // Thread t2(1, &t1, &m, &cv);

    // std::thread first(&ioHandler::handle, io, &t1);
    // std::thread second(&Draw::drawGraphics, draw, &window, &t2);

    while (window.isOpen()) 
    {

        // first.join();
        // second.join();

        TimeHandler::getInstance() -> updateDeltaTime();
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // first = std::thread(&ioHandler::handle, io, &t1);
        // second = std::thread(&Draw::drawGraphics, &window, &t2);

        // Start threads
        // first(ioHandler::handle(), *io, &t1);
        // second(drawGraphics(&window), &t2);

        io->handle();

        draw -> drawGraphics(&window);
        draw -> startMovements();
    }

    // Make sure both threads are complete before stopping main thread
    // first.join();
    // second.join();

    delete io;
    return 0;
}