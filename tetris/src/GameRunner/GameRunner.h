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
        int characterId;
        
        sf::Font font;
        sf::Text lineNumber;
        // sf::Text pause;
        sf::Text start;

        sf::RectangleShape gridBounds;
        ClientGameState* game;

    public:
        static GameRunner *getInstance(int id = 0);
        sf::RenderWindow *getWindow();
        void drawGraphics();
        void deserialize(std::string data);
        int getCharacterId();
        void adjustView();

        sf::Text getStartText();
};

#endif