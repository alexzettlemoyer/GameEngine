#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>
#include <string>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"

class GameState
{
    private:
        GameState();
        static GameState *instancePtr;
        float timescale;
        std::list<std::shared_ptr<GraphicsObject>> graphicsObjects;
        std::list<std::shared_ptr<Character>> characters;

    public:
        static GameState* getInstance();
        void updateGameState();
        void setupGameState();
        void getState();
        std::list<std::shared_ptr<GraphicsObject>> getGraphicsObjects();
        std::list<std::shared_ptr<Character>> getCharacters();
        std::shared_ptr<Character> newCharacter();
        std::string serialize();
        void deserialize(std::string data);
};

#endif
