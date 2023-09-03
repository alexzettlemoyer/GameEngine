#include <SFML/Graphics.hpp>
#include "graphics/draw.hpp"
#include "io/ioHandler.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Glennwood Mania", sf::Style::Default);

    setupGraphics();

    ioHandler *io = new ioHandler(character);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        io->handle(&window);

        drawGraphics(&window);
        startMovements();
    }

    delete io;
    return 0;
}