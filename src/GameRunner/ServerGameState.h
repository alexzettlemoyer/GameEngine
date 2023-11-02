#ifndef SERVER_GAME_STATE_H
#define SERVER_GAME_STATE_H

#include "GameState.h"
#include "../Events/Eventhandler.h"

class ServerGameState : public GameState
{
    private:
        ServerGameState();
        static ServerGameState* instancePtr;
        EventHandler* eventHandler;

    public:
        static ServerGameState* getInstance();
        void setupServerGameState();
        void updateGameState();
        std::string serialize();
        float getDt();
        void input(std::string objId, std::string i);
        int newCharacter();
        void updateCharacterPosition(std::string charId, float x, float y );
};

#endif