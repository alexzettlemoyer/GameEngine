#include "Piece.h"
#include <iostream>
#include "../Scripting/v8helpers.h"
#include "../GameRunner/ClientGameState.h"

static const std::string TILES = "images/tiles.jpg";
static const sf::Vector2f BLOCK_SIZE = sf::Vector2f(25.f, 25.f);

std::map<char, std::array<unsigned short, 4>> blockPositionsMap = {
    {'i', {0x0F00, 0x2222, 0x00F0, 0x4444}},
    {'j', {0x44C0, 0x8E00, 0x6440, 0x0E20}},
    {'l', {0x4460, 0x0E80, 0xC440, 0x2E00}},
    {'o', {0xCC00, 0xCC00, 0xCC00, 0xCC00}},
    {'s', {0x06C0, 0x8C40, 0x6C00, 0x4620}},
    {'t', {0x0E40, 0x4C40, 0x4E00, 0x4640}},
    {'z', {0x0C60, 0x4C80, 0xC600, 0x2640}}
 };
std::map<char, sf::Color> colorMap = {
    {'i', sf::Color(sf::Color::Cyan)},
    {'j', sf::Color(sf::Color::Blue)},
    {'l', sf::Color(sf::Color(255, 165, 0))},
    {'o', sf::Color(sf::Color::Yellow)},
    {'s', sf::Color(sf::Color::Green)},
    {'t', sf::Color(sf::Color::Magenta)},
    {'z', sf::Color(sf::Color::Red)}
};

bool loadTexture(sf::Texture& texture, const std::string& image, sf::IntRect rect)
{
    std::cout << "loading" << std::endl;
    if (!texture.loadFromFile(image, rect))
        return false;
    return true;
}

Piece::Piece(char type, int x, int y)
{
    this -> type = type;
    this -> index = index;
    this -> currentRotation = 0;

    this -> blockPositions = blockPositionsMap[type];

    this-> coordinate = sf::Vector2f(x, y);
    previousX = x;
    previousY = y;

    grounded = false;
    // this -> place(x, y);

    // int startingCorner = types[type] * 18;
    // texture = std::make_shared<sf::Texture>();
    // if (!loadTexture(*texture, TILES.c_str(), sf::IntRect(startingCorner, 0, 17, 17)))
    //     { }
    
    for (int i = 0; i < 4; i++)
    {
        std::shared_ptr<sf::RectangleShape> currentBlock = std::make_shared<sf::RectangleShape>();
        // currentBlock -> setTexture(texture.get());
        currentBlock -> setSize(BLOCK_SIZE);
        currentBlock -> setFillColor(colorMap[type]);
        currentBlock -> setOutlineColor(sf::Color::Black);
        currentBlock -> setOutlineThickness(1.f);

        blocks.push_back(currentBlock);
    }

    this -> setPosition(x, y, currentRotation);

}

std::vector<std::shared_ptr<sf::RectangleShape>> Piece::getBlocks()
{
    return blocks;
}

bool Piece::isGrounded()
{
    std::lock_guard<std::mutex> lock(pieceMutex);

    std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> grid = ClientGameState::getInstance() -> getGrid();
    unsigned int pattern = blockPositions[ currentRotation ];

    std::vector<sf::Vector2f> blockCoordinates;

    for (int i = 0; i < 16; ++i) {
        if (pattern & (1 << (15 - i))) {
            int blockX = i % 4;
            int blockY = i / 4;

            int gridX = blockX + coordinate.x;
            int gridY = blockY + coordinate.y;

            if ( gridY >= ClientGameState::ROW_COUNT - 1 )
                return true;

            blockCoordinates.push_back(sf::Vector2f(gridX, gridY));            
        }
    }

    for (sf::Vector2f coord : blockCoordinates)
    {
        auto it = std::find(blockCoordinates.begin(), blockCoordinates.end(), sf::Vector2f(coord.x, coord.y + 1));
        if (it == blockCoordinates.end() && (*grid)[coord.y + 1][coord.x] != nullptr )
            return true;
    }

    return false;
}

bool Piece::isValidMove(int x, int y, unsigned short piecePattern)
{

   std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> grid = ClientGameState::getInstance() -> getGrid();

    for (int i = 0; i < 16; ++i) {
        if (piecePattern & (1 << (15 - i))) {
            int blockX = i % 4;
            int blockY = i / 4;

            int gridX = blockX + x;
            int gridY = blockY + y;

            // if the move is out of grid bounds
            if (gridX < 0 || gridX >= ClientGameState::COLS || gridY >= ClientGameState::ROW_COUNT)
            {
                std::cout << "out of bounds" << std::endl;
                return false;
            }

            // if the move would overlap with another piece
            if ((*grid)[gridY][gridX] != nullptr )
            {
                std::cout << "occupied" << std::endl;
                return false;
            }
        }
    }
    return true;
}   

bool Piece::setPosition(int x, int y, int rotation)
{
    std::lock_guard<std::mutex> lock(pieceMutex);

    unsigned short pattern = blockPositions[ rotation ];

    removeCellsFromGrid( previousX, previousY, previousPattern );

    if (isValidMove(x, y, pattern))
    {
        this -> coordinate = sf::Vector2f(x, y);

        int blockIndex = 0;
        for (int i = 0; i < 16; ++i) {
            if (pattern & (1 << (15 - i))) {

                int blockX = i % 4;
                int blockY = i / 4;

                float newPositionX = 15.f + BLOCK_SIZE.x * ( x + blockX );
                float newPositionY = 15.f + BLOCK_SIZE.y * ( y + blockY );

                blocks[blockIndex]->setPosition(sf::Vector2f(newPositionX, newPositionY));
                blockIndex++;

                addCellsToGrid(x, y, pattern);
                previousX = x;
                previousY = y;
                previousPattern = pattern;
            }
        }
        return true;
    }
    else
        addCellsToGrid( previousX, previousY, previousPattern );
    return false;
}

void Piece::removeCellsFromGrid(int x, int y, unsigned short piecePattern) 
{
    std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> grid = ClientGameState::getInstance() -> getGrid();

    for (int i = 0; i < 16; ++i) {
        if (piecePattern & (1 << (15 - i))) {
            int blockX = i % 4;
            int blockY = i / 4;

            int gridX = x + blockX;
            int gridY = y + blockY;

            (*grid)[gridY][gridX] = nullptr;
        }
    }
}

void Piece::addCellsToGrid(int x, int y, unsigned short piecePattern) 
{
    std::shared_ptr<std::vector<std::vector<std::shared_ptr<sf::RectangleShape>>>> grid = ClientGameState::getInstance() -> getGrid();

    int blockIndex = 0;

    for (int i = 0; i < 16; ++i) {
        if (piecePattern & (1 << (15 - i))) {
            int blockX = i % 4;
            int blockY = i / 4;

            int gridX = x + blockX;
            int gridY = y + blockY;

            (*grid)[gridY][gridX] = blocks[blockIndex];
            blockIndex += 1;
        }
    }
}

sf::Vector2f Piece::getCoordinate()
{
    return coordinate;
}

void Piece::setRotation(int dir)
{
    if (setPosition(previousX, previousY, dir))
        this -> currentRotation = dir;
    else
        std::cout << "invalid rotation" << std::endl;
}

int Piece::getRotation()
{
    return currentRotation;
}

v8::Local<v8::Object> Piece::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("rotation", getPieceRotation, setPieceRotation));
    v.push_back(v8helpers::ParamContainer("x", getPieceX, setPieceX));
    v.push_back(v8helpers::ParamContainer("y", getPieceY, setPieceY));

	return v8helpers::exposeToV8("piece", this, v, isolate, context, context_name);
}

void Piece::setPieceRotation(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    int32_t newRotation = value->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);

    Piece* obj = static_cast<Piece*>(ptr);
    obj -> setRotation(newRotation);
}

void Piece::getPieceRotation(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    Piece* obj = static_cast<Piece*>(ptr);
    int currentRotation = obj->getRotation();

	info.GetReturnValue().Set(currentRotation);
}

void Piece::getPieceX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    Piece* obj = static_cast<Piece*>(ptr);
    sf::Vector2f currentPosition = obj->getCoordinate();

	int x_val = int(currentPosition.x);
	info.GetReturnValue().Set(x_val);
}

void Piece::setPieceX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    int32_t xValue = value->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);
    Piece* obj = static_cast<Piece*>(ptr);

    sf::Vector2f currentPosition = obj->getCoordinate();
    int rotation = obj->getRotation();
    obj->setPosition(xValue, currentPosition.y, rotation);
}

void Piece::getPieceY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    Piece* obj = static_cast<Piece*>(ptr);
    sf::Vector2f currentPosition = obj->getCoordinate();

	int y_val = int(currentPosition.y);
	info.GetReturnValue().Set(y_val);
}

void Piece::setPieceY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    int32_t yValue = value->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);
    Piece* obj = static_cast<Piece*>(ptr);

    sf::Vector2f currentPosition = obj->getCoordinate();
    int rotation = obj -> getRotation();
    obj->setPosition(currentPosition.x, yValue, rotation);
}


