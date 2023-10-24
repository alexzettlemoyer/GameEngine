#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>
#include <string>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/DeathZone.h"
#include "../GraphicsObject/SideBoundary.h"


class GameState
{
    private:
        GameState();
        static GameState *instancePtr;
        float timescale;
        std::shared_ptr<DeathZone> deathZone;
        std::list<std::shared_ptr<SideBoundary>> sideBoundaries;
        std::list<std::shared_ptr<GraphicsObject>> graphicsObjects;

    public:
            // raw game state: used by both client and server
        static GameState* getInstance();
        void setupGameState();
        std::list<std::shared_ptr<GraphicsObject>> getGraphicsObjects();
        std::list<Character*> getCharacters();
        int newCharacter();
        void removeObject(int id);
        std::shared_ptr<GraphicsObject> findObjById(int id);

            // server game state

        void updateGameState();
        std::string serialize();
        static std::vector<std::string> split(const std::string& str, char delimiter);
        std::shared_ptr<DeathZone> getDeathZone();
        void input(std::string objId, std::string i);


            // client game state
        void deserialize(std::string data);
        std::list<std::shared_ptr<SideBoundary>> getSideBoundaries();
        void checkSideCollision(int characterId);
        void scrollObjects();

        
            // ?
        void getState();
};

#endif
