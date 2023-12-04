#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include <mutex>
#include <v8/v8.h>

class Piece
{
    public:
        Piece(char, int, int);
        std::vector<std::shared_ptr<sf::RectangleShape>> getBlocks();
        std::shared_ptr<sf::Texture> texture;

        sf::Vector2f getCoordinate();
        void setRotation(int dir);
        int getRotation();
        bool isGrounded();

        v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");

    private:
        std::mutex pieceMutex;
        std::vector<std::shared_ptr<sf::RectangleShape>> blocks;
        std::array<unsigned short, 4> blockPositions;

        int previousX;
        int previousY;
        unsigned short previousPattern;

        bool grounded;
        char type;
        int index;
        int currentRotation;

        sf::Vector2f position;
        sf::Vector2f coordinate;

        bool isValidMove(int x, int y, unsigned short piecePattern);
        bool setPosition(int x, int y, int rotation);
        void addCellsToGrid(int x, int y, unsigned short piecePattern);
        void removeCellsFromGrid(int x, int y, unsigned short piecePattern);

        static void setPieceRotation(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void getPieceRotation(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void getPieceX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void setPieceX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
        static void getPieceY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void setPieceY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
};

#endif