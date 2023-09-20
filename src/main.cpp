#include <SFML/Graphics.hpp>
#include "Draw/Draw.hpp"
#include "io/ioHandler.h"
#include "Time/Timeline.h"
#include "Time/Thread.cpp"
#include <iostream>


int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default);
    window.setFramerateLimit(120);

    Draw *draw = Draw::getInstance();
    draw -> setupGraphics(&window);

    ioHandler *io = ioHandler::getInstance(draw -> character);

    // Mutex to handle locking, condition variable to handle notifications between threads
    // std::mutex m;
    // std::condition_variable cv;

    // Create thread objects
    // Thread t1(0, nullptr, &m, &cv, [&io]() { io->handle(); });
    // Thread t2(1, &t1, &m, &cv, [&draw]() {draw->startMovements(); }); 

    // Thread t1(0, nullptr, &m, &cv, [&io](){ io->handle(); }, std::ref(io));
    // Thread t2(1, &t1, &m, &cv, [&draw](){ draw->startMovements(); }, std::ref(draw));

    // Initialize threads
    // std::thread first;
    // std::thread second;

    while (window.isOpen()) 
    {
        Timeline::getInstance() -> updateDeltaTime();
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Key::P)
                    Timeline::getInstance() -> pause();
        }

            // *** Keep draw -> drawGraphics(&window) in main thread
            // *** SFML does not support rendering through a thread
        draw -> drawGraphics(&window);

        if (!(Timeline::getInstance() -> isPaused()))
        {
            io -> handle();
            draw -> startMovements();
        }
        
        // start the thread for io handling
        // first = std::thread(&Thread::run, &t1);

        // start the thread for object movements
        // second = std::thread(&Thread::run, &t2);

        // Join threads after each iteraion
        // if (first.joinable())
        //     first.join();
        // if (second.joinable())
        //     second.join();
    }
       
    //    // Make sure both threads are complete before stopping main thread
    // first.join();
    // second.join();

    delete draw;
    delete io;
    return 0;
}