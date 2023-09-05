#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"

sf::FloatRect nextPosition;

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

    nextPosition = characterBounds;
    sf::Vector2f velocity = obj.getVelocity();
    nextPosition.left += velocity.x;
    nextPosition.top += velocity.y;

    if (otherBounds.intersects(nextPosition))
    {
        // std::cout << "colliding" << std::endl;
    //     // push the character away from the moving object
    //     if ( push == 1.0f )
    //     {
    //         std:: cout << "collision" << std::endl;
    //         // calculate the center of both objects
    //         // bounds.left give current x position of the obj + width = top length / 2 = center x
    //         // bounds.top give current y position of the obj + height = side length / 2 = center y

    //         // sf::Vector2f characterCenter(
    //         //     characterBounds.left + characterBounds.width / 2.0f,
    //         //     characterBounds.top + characterBounds.height / 2.0f
    //         // );

    //         // sf::Vector2f otherCenter(
    //         //     otherBounds.left + otherBounds.width / 2.0f,
    //         //     otherBounds.top + otherBounds.height / 2.0f
    //         // );       

    //         // // get the direction of the collision
    //         // sf::Vector2f collisionDirection = characterCenter - otherCenter;
    //         // std::cout << collisionDirection.x << ", " << collisionDirection.y << std::endl;
    //         // // -117.467, 41.5513

    //         // // calculate magnitude of collision
    //         // float collisionLength = sqrt(collisionDirection.x * collisionDirection.x + collisionDirection.y * collisionDirection.y);

    //         // if (collisionLength != 0.0f)
    //         // {
    //         //     collisionDirection /= collisionLength;  // get Unit vector
                
    //         //     // collision depth: how far objs intersect
    //         //     float collisionDepth = characterBounds.width / 2.0f + otherBounds.width / 2.0f - collisionLength;
                
    //         //     sf::Vector2f pushVector = collisionDirection * collisionDepth;
    //         //     obj.move(pushVector.x, pushVector.y);
    //         // }
    //     }
        return true;
    }

//     sf::Vector2f thisPosition = obj.getPosition();
//     sf::Vector2f thisHalfSize = obj.getSize() / 2.0f;

//     sf::Vector2f otherPosition = other.getPosition();
//     sf::Vector2f otherHalfSize = other.getSize() / 2.0f;

//     float deltaX = otherPosition.x - thisPosition.x;
//     float deltaY = otherPosition.y - thisPosition.y;
//     float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
//     float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

//    //  std::cout << intersectX << ", " << collisionDirection.y << std::endl;


//     // collision
//     if (intersectX < 0.0f && intersectY < 0.0f)
//     {
        // push = std::min(std::max(push, 0.0f), 1.0f)
        // if ( push == 1.0f )
        // {
        //     if ( intersectX > intersectY )
        //     {
        //         if ( deltaX > 0.0f )
        //         {
        //             obj.move(intersectX * (1.0f - push), 0.0f);
        //             other.move(-intersectX * push, 0.0f);
        //         }
        //         else
        //         {
        //             obj.move(-intersectX * (1.0f - push), 0.0f);
        //             other.move(intersectX * push, 0.0f);
        //         }
        //     }
        //     else
        //     {
        //         if ( deltaY > 0.0f )
        //         {
        //             obj.move(0.0f, intersectY * (1.0f - push));
        //             other.move(0.0f, -intersectY * push);
        //         }
        //         else
        //         {
        //             obj.move(0.0f, -intersectY * (1.0f - push));
        //             other.move(0.0f, intersectY * push);
        //         }
        //     }
        // }
    //     return true;
    // }
    // std::cout << thisPosition.x << ", " << thisPosition.y << std::endl;
    // std::cout << otherPosition.x << ", " << otherPosition.y << std::endl;

    return false;
}
