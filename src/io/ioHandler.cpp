#include <SFML/Graphics.hpp>
#include "ioHandler.h"
#include <memory>

std::shared_ptr<Character> characterRef;

ioHandler::ioHandler(std::shared_ptr<Character> c)
{
    characterRef = c;
}

void ioHandler::handle(sf::RenderWindow* window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        characterRef->up(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        characterRef->up(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        characterRef->down(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        characterRef->right(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        characterRef->left(window);
}