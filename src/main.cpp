#include <SFML/Graphics.hpp>
#include "graphics/draw.hpp"
#include "io/ioHandler.h"
#include <iostream>

int main()
{
    float dt;
    sf::Clock dt_clock;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default);
    window.setFramerateLimit(120);

    setupGraphics();

    ioHandler *io = new ioHandler(character);

    while (window.isOpen()) 
    {
        dt = dt_clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        io->handle(&window, dt);
       // std::cout << "1" << std::endl;

        drawGraphics(&window);
       // std::cout << "2" << std::endl;

        startMovements();
        // std::cout << "3" << std::endl;
    }

    delete io;
    return 0;
}