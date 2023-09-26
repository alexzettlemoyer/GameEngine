#ifndef GAME_RUNNER_H
#define GAME_RUNNER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <atomic>
#include <list>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"

class GameRunner
{
    private:
        GameRunner();
        static GameRunner* instancePtr;
        std::atomic<bool> isGameRunning;
        sf::RenderWindow window;
        std::list<std::shared_ptr<GraphicsObject>> graphicsObjects;
        std::list<std::shared_ptr<Character>> characters;
        sf::Sprite background;
        sf::Texture backgroundTexture;

    public:
        static GameRunner *getInstance();
        sf::RenderWindow *gameLoop(sf::RenderWindow *window);
        sf::RenderWindow *getWindow();
        void drawGraphics();
};

#endif
