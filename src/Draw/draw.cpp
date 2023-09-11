#include <SFML/Graphics.hpp>
#include <list>
#include "Draw.hpp"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Item.h"
#include "../Movement/Mover.hpp"

std::shared_ptr<Character> character = std::make_shared<Character>(sf::Vector2f(100.f, 180.f));

std::list<std::shared_ptr<GraphicsObject>> graphicsObjects;
std::list<std::shared_ptr<Platform>> platforms;
std::shared_ptr<Platform> platform1 = std::make_shared<Platform>(sf::Vector2f(25.f, 520.f));
std::shared_ptr<Platform> platform2 = std::make_shared<Platform>(sf::Vector2f(525.f, 650.f));
std::shared_ptr<Item> item1 = std::make_shared<Item>(sf::Vector2f(100.2f, 64.6f), sf::Vector2f(800.f, 150.f));

static const std::string IMG_BACKGROUND = "images/background.jpeg";
sf::Sprite background;
sf::Texture backgroundTexture;

bool setupGraphics(sf::RenderWindow* window)
{
    if (!backgroundTexture.loadFromFile(IMG_BACKGROUND))
        return false;

    background.setScale(1.f, 1.f);
    background.setTexture(backgroundTexture);

    character.get() -> addWindowReference(window);
    graphicsObjects.push_back(platform1);
    graphicsObjects.push_back(platform2);
    graphicsObjects.push_back(item1);

    platforms.push_back(platform1);
    platforms.push_back(platform2);

    return true;
}


bool drawGraphics(sf::RenderWindow* window)
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

void startMovements()
{
    characterMovement(*character);
    movementClockwise(*item1);
    movementLeftRight(*platform2);
}

bool finalize()
{

    return true;
}