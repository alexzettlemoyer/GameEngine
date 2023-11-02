#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <mutex>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/SideBoundary.h"
#include "../Time/Timeline.h"
#include "../GameRunner/ServerGameState.h"
#include "../GameRunner/ClientGameState.h"
#include <iostream>

sf::FloatRect nextPosition;

bool isCharacterGrounded(Character &character, GraphicsObject &ground)
{

    sf::FloatRect characterBounds = character.getGlobalBounds();
    sf::FloatRect otherBounds = ground.getGlobalBounds();

    // if bottom x coordinate of character is below top x coordinate of platform
    // and the character is within the x coordinates of the platform ( on top )
    if (characterBounds.top + characterBounds.height >= otherBounds.top &&
        characterBounds.left + characterBounds.width >= otherBounds.left &&
        characterBounds.left <= otherBounds.left + otherBounds.width)
            return true;

    return false;
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

bool checkCollision(Character &obj, GraphicsObject &other, bool withResponse)
{
    sf::FloatRect characterBounds = obj.getGlobalBounds();
    sf::FloatRect otherBounds = other.getGlobalBounds();

    // reduce character height by 5.f
    // this is so that the character is not detected colliding with a platform
    // when it is visually not on the platform
    characterBounds.height -= 10.f;

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

        int collisionDir;
        // x axis collision
        if ( width < height )
            collisionDir = 0;
        // y axis collision
        else if ( width > height ) 
            collisionDir = 1;           // y collision - do nothing for now
        else { }                         // perfect x=y collision - do nothing for now
        
        if ( withResponse )
        {
            std::shared_ptr<Event> e = std::make_shared<Event>(Event::COLLISION, obj.getTimeline() -> getTimeStamp());
            e -> addCharacterVariant(&obj);
            e -> addGraphicsObjectVariant(&other);
            e -> addCollisionDirectionVariant( collisionDir );

            EventHandler::getInstance() -> onEvent(e);
        }
        return true;
    }
    
    return false;
}
