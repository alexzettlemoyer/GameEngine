#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>
#include <string>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/DeathZone.h"
#include "../GraphicsObject/SideBoundary.h"
#include "../GraphicsObject/Piece.h"


class GameState
{
    protected:
        GameState();
        static int objectId;
        std::shared_ptr<Timeline> timeline;
        std::mutex stateMutex;
        float timescale;
        std::list<std::shared_ptr<GraphicsObject>> graphicsObjects;

    public: // raw game state: used by both client and server

        std::unordered_map<char, std::shared_ptr<Piece>> pieces;

        static GameState* getInstance();
        void setupGameState();
        std::list<std::shared_ptr<GraphicsObject>> getGraphicsObjects();
        std::shared_ptr<GraphicsObject> findObjById(int id);
        std::list<Character*> getCharacters();
        std::vector<std::string> split(const std::string& str, char delimiter);
        int newCharacter();
        void removeObject(int id);
};

#endif
