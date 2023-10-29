#ifndef GAME_RUNNER_H
#define GAME_RUNNER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <atomic>
#include <list>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "ClientGameState.h"

class GameRunner
{
    private:
        GameRunner(int clientId);
        static GameRunner* instancePtr;
        std::atomic<bool> isGameRunning;
        sf::RenderWindow window;
        sf::RectangleShape background;
        // sf::Sprite background;
        // sf::Texture backgroundTexture;
        int characterId;
            // create a view with its center and size
        sf::View view = sf::View(sf::Vector2f(500.f, 400.f), sf::Vector2f(1000.f, 800.f));
        ClientGameState* game;

    public:
        static GameRunner *getInstance(int id = 0);
        sf::RenderWindow *getWindow();
        void drawGraphics();
        void deserialize(std::string data);
        int getCharacterId();
        void adjustView();
};

#endif