#include <SFML/Graphics.hpp>
#include "ioHandler.h"
#include <memory>

std::shared_ptr<Character> characterRef;

ioHandler::ioHandler(std::shared_ptr<Character> c)
{
    characterRef = c;
}

void ioHandler::handle(sf::RenderWindow* window, float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        characterRef->up(window, dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        characterRef->up(window, dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        characterRef->down(window, dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        characterRef->right(window, dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        characterRef->left(window, dt);
}