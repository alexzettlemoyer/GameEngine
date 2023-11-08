#ifndef CLIENT_GAME_STATE
#define CLIENT_GAME_STATE

#include "GameState.h"

class ClientGameState : public GameState
{
    private: 
        ClientGameState(int id);
        static ClientGameState* instancePtr;
        std::list<std::shared_ptr<SideBoundary>> sideBoundaries;
        std::shared_ptr<DeathZone> deathZone;
        std::shared_ptr<Character> thisCharacter;
        float dt;
        float ticSize;

    public:
        static ClientGameState* getInstance(int id = 0);
        void setupClientGameState(int id);
        void updateGameState();
        void deserialize(std::string data, int characterId);
        std::list<std::shared_ptr<SideBoundary>> getSideBoundaries();
        float getDt();
        float getTicSize();
        float getTimeStamp();
        std::shared_ptr<DeathZone> getDeathZone();
        sf::Vector2f getCharacterPosition();
        std::shared_ptr<Character> getCharacter();
};

#endif