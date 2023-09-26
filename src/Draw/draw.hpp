#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"
class Draw
{
    private:
        Draw();
        static Draw* instancePtr;
        sf::Sprite background;
        sf::Texture backgroundTexture;

    public:
        static Draw *getInstance();
        std::list<std::shared_ptr<GraphicsObject>> graphicsObjects;
        std::list<std::shared_ptr<Character>> characters;
        std::shared_ptr<Character> character;
        bool setupGraphics(sf::RenderWindow* window);
        bool drawGraphics(sf::RenderWindow* window);
        void startMovements(std::atomic<bool>& isGameRunning);
        std::shared_ptr<Character> newCharacter();
};
#endif