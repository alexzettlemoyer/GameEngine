#include <SFML/Graphics.hpp>
#include "ClientGameState.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"
#include "../GraphicsObject/Character.h"
#include "../Events/EventHandler.h"
#include <set>
#include <iostream>

ClientGameState* ClientGameState::instancePtr = nullptr;

ClientGameState::ClientGameState(int id) : grid(std::make_shared<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>>(16, std::vector<std::shared_ptr<sf::RectangleShape>>(10, nullptr)))
{
    setupClientGameState(id);
}

ClientGameState* ClientGameState::getInstance(int id)
{
    if (instancePtr == NULL)
        { instancePtr = new ClientGameState(id); }
    return instancePtr;
}

void ClientGameState::setupClientGameState(int id)
{
    std::lock_guard<std::mutex> lock(stateMutex);

    dt = 0.01;
    elapsedTime = 0;

    lineCount = 0;
    playing = false;
}

void ClientGameState::addScriptManager(std::shared_ptr<ScriptManager> sm)
{
    scriptManager = sm;
}

int ClientGameState::getLineCount()
{
    return lineCount;
}

void ClientGameState::startGame()
{
    grid = std::make_shared<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>>(16, std::vector<std::shared_ptr<sf::RectangleShape>>(10, nullptr));
    lineCount = 0;
    playing = true;
}

void ClientGameState::updateGameState()
{
    EventHandler::getInstance() -> handleEvents();

    if (playing)
    {
        elapsedTime += dt;

        if (currentPiece != nullptr && currentPiece -> isGrounded())
        {
            checkCompletedLines();
            if (currentPiece -> getCoordinate().y <= 1 )
                playing = false;
            else
                scriptManager -> runOne("raise_new_piece_event", false, "object_context");
        }
                
        if ( playing && elapsedTime >= STEP && currentPiece != nullptr )
        {
            scriptManager -> runOne("raise_down_event", false, "object_context");
            elapsedTime = 0;
        }

        STEP = 0.1f - (lineCount / 500.f);
    }
}

void printGrid(std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> grid)
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if ((*grid)[i][j] == nullptr)
                std::cout << "0 ";
            else
                std::cout << "X ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}


void ClientGameState::checkCompletedLines()
{
    std::lock_guard<std::mutex> lock(stateMutex);

    std::vector<int> completedRows;
    for ( int i = ClientGameState::ROW_COUNT - 1; i >= 0; i--)
    {
        bool rowComplete = true;
        for (int j = 0; j < ClientGameState::COLS; j++)
        {
            if ((*grid)[i][j] == nullptr)
                rowComplete = false;
        }
        if (rowComplete)
            completedRows.push_back( i );
    }

    lineCount += completedRows.size();

    for ( int completedIndex : completedRows )
    {
        // erase the completed row
        for (int j = 0; j < ClientGameState::COLS; j++ )
        {
            (*grid)[completedIndex][j] = nullptr;
        }
    }

    for ( int completedIndex : completedRows )
    {
        for ( int i = completedIndex - 1; i >= 0; i-- )
        {
            for ( int j = 0; j < ClientGameState::COLS; j++ )
            {
                std::shared_ptr<sf::RectangleShape> currentBlock = (*grid)[i][j];
                if (currentBlock != nullptr)
                {
                    sf::Vector2f currentPosition = currentBlock->getPosition();
                    float newYPosition = (static_cast<float>(i + completedRows.size()) * 25.f) + 15.f;

                    if (i + completedRows.size() < ClientGameState::ROW_COUNT)
                    {
                        currentBlock->setPosition(currentPosition.x, newYPosition);

                        (*grid)[i + completedRows.size()][j] = currentBlock;
                        (*grid)[i][j] = nullptr;
                    }
                }
            }
        }
    }

    if (completedRows.size() > 0)
        printGrid(grid);
}

std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> ClientGameState::getGrid()
{
    return grid;
}

std::shared_ptr<Piece> ClientGameState::getCurrentPiece()
{
    return currentPiece;
}

void ClientGameState::newPiece( std::string t )
{
    std::lock_guard<std::mutex> lock(stateMutex);

    char type = t[0];

    std::shared_ptr<Piece> piece = std::make_shared<Piece>(type, 3, 0);

    currentPieces.push_back(piece);
    currentPiece = piece;
}

std::list<std::shared_ptr<Piece>> ClientGameState::getCurrentPieces()
{
    return currentPieces;
}

sf::Vector2f ClientGameState::getCharacterPosition()
{
    return sf::Vector2f(thisCharacter -> getPosition());
}

std::shared_ptr<Character> ClientGameState::getCharacter()
{
    return thisCharacter;
}

void ClientGameState::deserialize(std::string data, int characterId)
{
    std::vector<std::string> objs = split(data, ']');
    std::set<int> currentGameIds;

    // get delta time
    std::vector<std::string> timelineInfo = split(objs[ 0 ], ' ');

    std::lock_guard<std::mutex> lock(stateMutex);
    dt = stof(timelineInfo[ 1 ]);
    ticSize = stof(timelineInfo[2]);
}

std::list<std::shared_ptr<SideBoundary>> ClientGameState::getSideBoundaries()
{
    return sideBoundaries;
}

float ClientGameState::getDt()
{
    return dt;
}

float ClientGameState::getTicSize()
{
    return ticSize;
}

float ClientGameState::getTimeStamp()
{
    return timeline -> getTimeStamp();
}

std::shared_ptr<DeathZone> ClientGameState::getDeathZone()
{
    return deathZone;
}

std::string ClientGameState::getNextPiece()
{
    std::string value = newPieces.front();
    newPieces.pop();
    return value;
}

// void ClientGameState::setStep(float newTimeStep)
// {
//     this -> STEP = newTimeStep;
// }

v8::Local<v8::Object> ClientGameState::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("newpiece", getNewPiece, setNewPiece));
    v.push_back(v8helpers::ParamContainer("playing", getPlaying, setPlaying));
    v.push_back(v8helpers::ParamContainer("start", getPlaying, setGameStart));
    v.push_back(v8helpers::ParamContainer("lines_completed", getLinesCompleted, setLinesCompleted));
    v.push_back(v8helpers::ParamContainer("time_step", getTimeStep, setTimeStep));

	return v8helpers::exposeToV8("game_state", this, v, isolate, context, context_name);
}

void ClientGameState::setNewPiece(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
    v8::Local<v8::String> newPieceString = value->ToString(isolate->GetCurrentContext()).ToLocalChecked();
    v8::String::Utf8Value utf8Value(isolate, newPieceString);
    std::string newPieceStr = *utf8Value;

    // ClientGameState* obj = static_cast<ClientGameState*>(ptr);
    ClientGameState::getInstance() -> newPiece(newPieceStr);
}

void ClientGameState::getNewPiece(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    std::string nextPiece = ClientGameState::getInstance() -> getNextPiece();
    v8::Local<v8::String> nextPieceStr = v8::String::NewFromUtf8(isolate, nextPiece.c_str());

	info.GetReturnValue().Set(nextPieceStr);
}

void ClientGameState::setPlaying(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    // bool newPlayingValue = value->BooleanValue(isolate);
    v8::Maybe<bool> maybeBool = value->BooleanValue(isolate->GetCurrentContext());
    bool newPlayingValue = maybeBool.FromJust();

    ClientGameState::getInstance() -> playing = newPlayingValue;
}

void ClientGameState::setGameStart(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    // bool newPlayingValue = value->BooleanValue(isolate);
    v8::Maybe<bool> maybeBool = value->BooleanValue(isolate->GetCurrentContext());
    bool newPlayingValue = maybeBool.FromJust();

    ClientGameState::getInstance() -> startGame();
}

void ClientGameState::getPlaying(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    bool isPlaying = ClientGameState::getInstance() -> playing;
	info.GetReturnValue().Set(isPlaying);
}

void ClientGameState::setTimeStep(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    float newStep = static_cast<float>(value->NumberValue(isolate->GetCurrentContext()).FromMaybe(0));

    ClientGameState::getInstance() -> STEP = newStep;
}

void ClientGameState::getTimeStep(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    float currentTimeStep = ClientGameState::getInstance() -> STEP;
	info.GetReturnValue().Set(currentTimeStep);
}

void ClientGameState::setLinesCompleted(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{ }

void ClientGameState::getLinesCompleted(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    float currentLineCount = ClientGameState::getInstance() -> STEP;
	info.GetReturnValue().Set(currentLineCount);
}