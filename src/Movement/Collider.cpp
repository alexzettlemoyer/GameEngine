#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <chrono>
#include <iostream>
#include <cmath>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../Draw/Draw.hpp"
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
            return true;

    return false;
}

// STOP_MOVEMENT, ERASE, PUSH
// dir = 0: x axis
// // dir = 1: y axis
void stopMovement(GraphicsObject &obj, int dir = -1)
{
    if ( dir == 0 || dir == -1 )
        obj.velocity.x = 0.f;
    if ( dir == 1 || dir == -1 )
        obj.velocity.y = 0.f;  
}

void eraseObj(GraphicsObject &obj)
{   
    // remove drawn pointer from list
    graphicsObjects.remove_if([&obj](const std::shared_ptr<GraphicsObject>& ptr) {
        return ptr.get() == &obj;
    });

    // after 20s reappear ?
}

// x: 0
// y: 0
bool collisionResponse(GraphicsObject &obj, int dir)
{
    if ( dir == 0 )
    {
        switch (obj.collisionTypeX)
        {
            case GraphicsObject::STOP_MOVEMENT:
                stopMovement(obj, 0);
                break;
            case GraphicsObject::ERASE:
                eraseObj(obj);
                break;
            case GraphicsObject::PUSH:
                break;
            case GraphicsObject::NONE: 
                break;
            default:
                stopMovement(obj, 1);
                break;
        }
    }
    else if ( dir == 1 )
    {
        switch (obj.collisionTypeY)
        {
            case GraphicsObject::STOP_MOVEMENT:
                stopMovement(obj, 1);
                break;
            case GraphicsObject::ERASE:
                eraseObj(obj);
                break;
            case GraphicsObject::PUSH:
                break;
            default:
                stopMovement(obj, 1);
                break;
        }
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

bool checkCollision(GraphicsObject &obj, GraphicsObject &other)
{
    sf::FloatRect characterBounds = obj.getGlobalBounds();
    sf::FloatRect otherBounds = other.getGlobalBounds();

    // reduce character height by 5.f
    // this is so that the character is not detected colliding with a platform
    // when it is visually not on the platform
    characterBounds.height -= 5.f;

    // calculate the characters next position
    nextPosition = characterBounds;
    sf::Vector2f velocity = obj.getVelocity();
    nextPosition.left += velocity.x;
    nextPosition.top += velocity.y;

    // intersection rectangle between the 2 objects
    sf::FloatRect intersection;
    if (otherBounds.intersects(nextPosition, intersection))
    {
        float width = intersection.width;
        float height = intersection.height;

        // x axis collision
        if ( width < height )
        {
            // handle the non character objects
            if ( obj.collisionTypeX != GraphicsObject::CHAR )
                collisionResponse(obj, 0);
            if ( other.collisionTypeX != GraphicsObject::CHAR )
                collisionResponse(other, 0);
        }
        // y axis collision
        else if ( width > height ) {}   // y collision - do nothing for now
        else {}                         // perfect x=y collision - do nothing for now

        return true;
    }
    return false;
}
