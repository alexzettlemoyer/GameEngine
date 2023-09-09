#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <chrono>
#include <iostream>
#include <cmath>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../Draw/draw.hpp"
#include "../Time/TimeHandler.h"

sf::FloatRect nextPosition;

bool isCharacterGrounded(Character &character, Platform &platform)
{
    sf::FloatRect characterBounds = character.getGlobalBounds();
    sf::FloatRect otherBounds = platform.getGlobalBounds();

    // if bottom x coordinate of character is below top x coordinate of platform
    // and the character is within the x coordinates of the platform ( on top )
    if (characterBounds.top + characterBounds.height >= otherBounds.top &&
        characterBounds.left + characterBounds.width >= otherBounds.left &&
        characterBounds.left <= otherBounds.left + otherBounds.width)
        {
            // std::cout << "ground" << std::endl;
            return true;
        }

    // std::cout << "not" << std::endl;
    return false;
}

// STOP_MOVEMENT, ERASE, PUSH
void stopMovement(GraphicsObject &obj)
{
    obj.velocity.x = 0.f;
    obj.velocity.y = 0.f;  
}

// dir = 0: x axis
// dir = 1: y axis
void stopDirMovement(GraphicsObject &obj, int dir)
{
    // std::cout << dir << std::endl;
    if ( dir == 0 )
        obj.velocity.x = 0;
    if ( dir == 1 )
        obj.velocity.y = 0;
}

void eraseObj(GraphicsObject &obj)
{   
    // remove drawn pointer from list
    graphicsObjects.remove_if([&obj](const std::shared_ptr<GraphicsObject>& ptr) {
        return ptr.get() == &obj;
    });

    // after 20s reappear ?
}

bool collisionResponse(GraphicsObject &obj)
{
    switch (obj.collisionType)
    {
        case GraphicsObject::STOP_MOVEMENT:
            stopMovement(obj);
            break;
        case GraphicsObject::ERASE:
            eraseObj(obj);
            break;
        case GraphicsObject::PUSH:
            break;
        default:
            stopMovement(obj);
            break;
    }
    return true;
}

/**
 * Check Collision
 * https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
 *
 * @param push
 *  push = 0
 *      this object should be completely pushed out of the other
 *  push = 1
 *      other object should be completely pushed out of this
 *
 * @param mvmntDir ( optional parameter )
 *      indicates the other object's movement direction
 *      if colliding, the character will be pushed in this direction.
 *          left     0
 *          up       1
 *          right    2
 *          down     3
 *
 *  AABB : axis-aligned line bounding box
 */
bool checkCollision(GraphicsObject &obj, GraphicsObject &other, float push, float mvmntDir = -1.f)
{
    sf::FloatRect characterBounds = obj.getGlobalBounds();
    sf::FloatRect otherBounds = other.getGlobalBounds();

    characterBounds.height -= 5.f;

    nextPosition = characterBounds;
    sf::Vector2f velocity = obj.getVelocity();
    nextPosition.left += velocity.x;
    nextPosition.top += velocity.y;

    sf::FloatRect intersection;

    // add nextPosition offset
    if (otherBounds.intersects(nextPosition, intersection))
    {
        float width = intersection.width;
        float height = intersection.height;

        // std::cout << width << ", " << height << std::endl;
        if (obj.collisionType != GraphicsObject::CHAR )
            collisionResponse(obj);
        if (other.collisionType != GraphicsObject::CHAR )
            collisionResponse(other);
    
        if ( width < height )
        {
            // collision on x axis
            stopDirMovement(obj, 0);
        }
        else
        {
            // return true;
        }   // collision on Y axis (due to gravity). Do nothing for now
        return true;
    }
    return false;
}
