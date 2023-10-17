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
        GameRunner(int clientId);
        static GameRunner* instancePtr;
        std::atomic<bool> isGameRunning;
        sf::RenderWindow window;
        sf::Sprite background;
        sf::Texture backgroundTexture;
        int characterId;
            // create a view with its center and size
        sf::View view = sf::View(sf::Vector2f(400.f, 500.f), sf::Vector2f(1000.f, 800.f));

    public:
        static GameRunner *getInstance(int id = 0);
        sf::RenderWindow *getWindow();
        void drawGraphics();
        void deserialize(std::string data);
        std::string getState();
};

#endif
