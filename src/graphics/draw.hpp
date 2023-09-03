#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"
#include <memory>

    extern std::shared_ptr<Platform> platform1;
    extern std::shared_ptr<Platform> platform2;
    extern std::shared_ptr<Character> character;
    extern std::shared_ptr<Item> item1;

    bool setupGraphics();
    bool drawGraphics(sf::RenderWindow* window);
    void startMovements();

#endif