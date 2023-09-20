#include <SFML/Graphics.hpp>
#include "ioHandler.h"
#include "../Draw/Draw.hpp"
#include <memory>
#include <mutex>
#include <iostream>

ioHandler* ioHandler::instancePtr = nullptr;
std::shared_ptr<Character> characterRef;

ioHandler::ioHandler(std::shared_ptr<Character> c)
{
    if ( c == nullptr )
        throw std::runtime_error("ioHandler not initialized");
    characterRef = c;
}

// singleton ioHandler class
ioHandler* ioHandler::getInstance(std::shared_ptr<Character> c)
{
    if ( instancePtr == NULL )
    {
        instancePtr = new ioHandler(c);
    }
    return instancePtr;
}

void ioHandler::handle()
{
    // std::cout << "handling" << std::endl;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        characterRef->up();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        characterRef->up();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        characterRef->down();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        characterRef->right();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        characterRef->left();

        // update the characters' movement, ensuring gravity is applied in every frame
    characterRef -> updateMovement();
}