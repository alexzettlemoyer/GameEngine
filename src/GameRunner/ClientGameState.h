#ifndef CLIENT_GAME_STATE
#define CLIENT_GAME_STATE

#include "GameState.h"

class ClientGameState : public GameState
{
    private: 
        ClientGameState();
        static ClientGameState* instancePtr;
        std::list<std::shared_ptr<SideBoundary>> sideBoundaries;

    public:
        static ClientGameState* getInstance();
        void setupClientGameState();
        void deserialize(std::string data);
        std::list<std::shared_ptr<SideBoundary>> getSideBoundaries();
        void checkSideCollision(int characterId);
        void scrollObjects();
};

#endif