#ifndef SERVER_GAME_STATE_H
#define SERVER_GAME_STATE_H

#include "GameState.h"

class ServerGameState : public GameState
{
    private:
        ServerGameState();
        static ServerGameState* instancePtr;
        std::shared_ptr<DeathZone> deathZone;

    public:
        static ServerGameState* getInstance();
        void setupServerGameState();
        void updateGameState();
        std::string serialize();
        std::shared_ptr<DeathZone> getDeathZone();
        void input(std::string objId, std::string i);
};

#endif