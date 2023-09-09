#include <SFML/Graphics.hpp>
#include "Draw/draw.hpp"
#include "io/ioHandler.h"
#include "Time/TimeHandler.h"
#include <iostream>

// extern sf::Clock dt_clock;

int main()
{
    // float dt;
    // sf::Clock dt_clock;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default);
    window.setFramerateLimit(120);

    setupGraphics(&window);

    ioHandler *io = new ioHandler(character);

    while (window.isOpen()) 
    {
        TimeHandler::getInstance() -> updateDeltaTime();

        // dt = dt_clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        io->handle();

        drawGraphics(&window);
        startMovements();
    }

    delete io;
    return 0;
}