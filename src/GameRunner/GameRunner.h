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
        sf::Sprite background;
        sf::Texture backgroundTexture;

    public:
        static GameRunner *getInstance();
        sf::RenderWindow *getWindow();
        void drawGraphics();
        void deserialize(std::string data);
};

#endif
