#include <SFML/Graphics.hpp>
#include "ClientGameState.h"
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/Item.h"
#include "../GraphicsObject/Character.h"
#include "../Movement/SideScroller.h"
#include <set>
#include <iostream>


ClientGameState* ClientGameState::instancePtr = nullptr;

ClientGameState::ClientGameState()
{ 
    setupClientGameState();
}

ClientGameState* ClientGameState::getInstance()
{
    if (instancePtr == NULL)
        { instancePtr = new ClientGameState(); }
    return instancePtr;
}

void ClientGameState::setupClientGameState()
{
    sideBoundaries.push_back(std::make_shared<SideBoundary>(sf::Vector2f(750.f, 0.f), objectId++, timeline, SideBoundary::RIGHT));
    sideBoundaries.push_back(std::make_shared<SideBoundary>(sf::Vector2f(0.f, 0.f), objectId++, timeline, SideBoundary::LEFT));

}

/**
 * deserialize is used by clients
 * to process the graphics object data sent by the client
 * format is:
 *      each graphicsObject is contained in []
 *      [ id position.x position.y type ]
 * sets up the GameState using the data
 * add any unrecognized objects
 */
void ClientGameState::deserialize(std::string data)
{
    std::vector<std::string> objs = split(data, ']');
    std::set<int> currentGameIds;

    for ( std::string const& obj : objs )
    {
        std::vector<std::string> objData = split(obj, ' ');

        // ignore objData[0] -> '['
        int id = stoi(objData[1]);
        currentGameIds.insert(id);
        std::shared_ptr<GraphicsObject> currentObj = findObjById( id );

        // there is a new graphics object that this client hasn't stored yet
        if ( currentObj == NULL )
        {
            switch(stoi(objData[4]))
            {
                case GraphicsObject::CHARACTER_TYPE:
                    graphicsObjects.push_back(std::make_shared<Character>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id, timeline));
                    break;
                case GraphicsObject::PLATFORM_TYPE:
                    graphicsObjects.push_back(std::make_shared<Platform>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id, timeline));
                    break;
                case GraphicsObject::ITEM_TYPE:
                    graphicsObjects.push_back(std::make_shared<Item>(sf::Vector2f(stof(objData[2]), stof(objData[3])), id, timeline));
                    break;
                default:
                    std::cout << "Error in deserialization: object cannot be created. id: " << id << std::endl;
                    break;
            }
        }
        else
        {
            currentObj.get() -> setPosition( sf::Vector2f(stof(objData[2]), stof(objData[3])) );
            // if ( data.size() > 4 )
            // {
            //     data[ 5 ] // extra message
            // }
        }
    }

        // compare the ids given by the server to the current ids we have in our graphics object list
    if ( currentGameIds.size() != graphicsObjects.size() )
    {
        for ( std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects() )
        {
            // found the object we need to remove
            if ( currentGameIds.find(i->identifier()) == currentGameIds.end())
                removeObject(i -> identifier());
        }
    }
}

void ClientGameState::checkSideCollision(int characterId)
{
    std::shared_ptr<GraphicsObject> thisCharacter = findObjById( characterId );
    if ( thisCharacter == NULL )
        return;
    for (std::shared_ptr<SideBoundary> const& i : getSideBoundaries())
    {
        if (SideScroller::getInstance() -> checkSideCollision(thisCharacter.get(), i.get()))
            break;
    }
    scrollObjects();
}

void ClientGameState::scrollObjects()
{
    float totalScrollDistance = SideScroller::getInstance() -> getSideScrollDistance();
    for (std::shared_ptr<GraphicsObject> const& i : getGraphicsObjects()) 
    {
        sf::Vector2f currentCoords = i -> getPosition();
        i -> setPosition(currentCoords - sf::Vector2f( totalScrollDistance, 0.f ));
    }
}

std::list<std::shared_ptr<SideBoundary>> ClientGameState::getSideBoundaries()
{
    return sideBoundaries;
}