#include <SFML/Graphics.hpp>
#include <list>
#include <atomic>
#include "Draw.hpp"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Item.h"
#include "../Movement/Mover.hpp"
#include <iostream>

static const std::string IMG_BACKGROUND = "images/background.jpeg";
Draw* Draw::instancePtr = nullptr;

std::shared_ptr<Character> character;
std::shared_ptr<Platform> platform1;
std::shared_ptr<Platform> platform2;
std::shared_ptr<Item> item1;

int characterCount = 0;

// constructor
Draw::Draw()
{
    character = std::make_shared<Character>(sf::Vector2f(100.f, 180.f), 4);
    platform1 = std::make_shared<Platform>(sf::Vector2f(25.f, 520.f), 1);
    platform2 = std::make_shared<Platform>(sf::Vector2f(525.f, 650.f), 2);
    item1 = std::make_shared<Item>(sf::Vector2f(100.2f, 64.6f), sf::Vector2f(800.f, 150.f), 3);
}

Draw* Draw::getInstance()
{
    if ( instancePtr == NULL )
        { instancePtr = new Draw(); }
    return instancePtr;
}

bool Draw::setupGraphics(sf::RenderWindow* window)
{
    if (!backgroundTexture.loadFromFile(IMG_BACKGROUND))
        return false;

    background.setScale(1.f, 1.f);
    background.setTexture(backgroundTexture);

    character.get() -> addWindowReference(window);
    characters.push_back(character);

    graphicsObjects.push_back(platform1);
    graphicsObjects.push_back(platform2);
    graphicsObjects.push_back(item1);

    return true;
}

bool Draw::drawGraphics(sf::RenderWindow* window)
{
        // draw the background
    window -> draw(background);

        // draw the objects
    for (std::shared_ptr<GraphicsObject> const& i : graphicsObjects) {
        window -> draw(*i);
    }

        // Draw the player
    window -> draw(*character);

        // Display the drawing on the window
    window -> display();

    return true;
}

void Draw::startMovements(std::atomic<bool>& isGameRunning)
{
    // while (isGameRunning.load())
    // {
        movementClockwise(*item1);
        movementLeftRight(*platform2);
    // }
}
