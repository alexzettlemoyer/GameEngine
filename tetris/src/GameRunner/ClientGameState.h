#ifndef CLIENT_GAME_STATE
#define CLIENT_GAME_STATE

#include "GameState.h"
#include "../Scripting/ScriptManager.h"
#include <queue>

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
        std::list<std::shared_ptr<Piece>> currentPieces;
        std::shared_ptr<Piece> currentPiece;

        std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> grid;

        float elapsedTime;
        std::shared_ptr<ScriptManager> scriptManager;

        void checkCompletedLines();
        std::string getNextPiece();
        
        static void setNewPiece(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void getNewPiece(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void setPlaying(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void getPlaying(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void setGameStart(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void setTimeStep(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void getTimeStep(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void setLinesCompleted(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void getLinesCompleted(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

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

        void addScriptManager(std::shared_ptr<ScriptManager> sm);

        std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> getGrid();

        std::list<std::shared_ptr<Piece>> getCurrentPieces();
        std::shared_ptr<Piece> getCurrentPiece();

        void newPiece(std::string t);
        int getLineCount();
        void startGame();
        void setStep(float newTimeStep);

        int lineCount;
        bool playing;

        float STEP = 0.1f;
        static const int COLS = 10;
        static const int ROW_COUNT = 16;

        std::queue<std::string> newPieces;

        v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");

};

#endif