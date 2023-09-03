#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "../GraphicsObject/Character.h"
#include <memory>

class ioHandler 
{
    public:
        ioHandler(std::shared_ptr<Character> c);
        void handle(sf::RenderWindow* window);
};

#endif