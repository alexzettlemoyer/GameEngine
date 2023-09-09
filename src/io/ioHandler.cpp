#include <SFML/Graphics.hpp>
#include "ioHandler.h"
#include "../Time/TimeHandler.h"
#include <memory>

std::shared_ptr<Character> characterRef;

ioHandler::ioHandler(std::shared_ptr<Character> c)
{
    characterRef = c;
}

void ioHandler::handle()
{
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
}