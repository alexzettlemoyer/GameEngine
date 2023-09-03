#include <SFML/Graphics.hpp>
#include "draw.hpp"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Item.h"
#include "../Movement/Mover.hpp"

std::shared_ptr<Platform> platform1 = std::make_shared<Platform>(sf::Vector2f(100.f, 520.f));
std::shared_ptr<Platform> platform2 = std::make_shared<Platform>(sf::Vector2f(525.f, 650.f));
std::shared_ptr<Character> character = std::make_shared<Character>();
std::shared_ptr<Item> item1 = std::make_shared<Item>(sf::Vector2f(100.2f, 64.6f));


bool setupGraphics()
{
    return true;
}


bool drawGraphics(sf::RenderWindow* window)
{
    // clear the window with light purple
    window -> clear(sf::Color(218, 178, 247));

    // draw the background
    // (*window).draw(background);

    // Draw the objects
    window -> draw(*platform1);
    window -> draw(*platform2);
    window -> draw(*character);
    window -> draw(*item1);

    // Display the drawing on the window
    window -> display();

    return true;

}

void startMovements()
{
    movementClockwise(*item1);
    movementLeftRight(*platform2);
}

bool finalize()
{

    return true;
}