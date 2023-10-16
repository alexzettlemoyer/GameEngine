#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>
#include <string>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/DeathZone.h"


class GameState
{
    private:
        GameState();
        static GameState *instancePtr;
        float timescale;
        std::shared_ptr<DeathZone> deathZone;
        std::list<std::shared_ptr<GraphicsObject>> graphicsObjects;
        std::shared_ptr<GraphicsObject> findObjById(int id);

    public:
        static GameState* getInstance();
        static std::vector<std::string> split(const std::string& str, char delimiter);
        void updateGameState();
        void setupGameState();
        void getState();
        std::list<std::shared_ptr<GraphicsObject>> getGraphicsObjects();
        std::list<Character*> getCharacters();
        std::shared_ptr<DeathZone> getDeathZone();
        int newCharacter();
        void removeObject(int id);
        std::string serialize();
        void deserialize(std::string data);
        void input(std::string objId, std::string i);
};

#endif
