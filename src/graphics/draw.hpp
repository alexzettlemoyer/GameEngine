#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"

    extern std::list<std::shared_ptr<GraphicsObject>> graphicsObjects;

    extern std::shared_ptr<Character> character;

    bool setupGraphics();
    bool drawGraphics(sf::RenderWindow* window);
    void startMovements(float dt);

#endif