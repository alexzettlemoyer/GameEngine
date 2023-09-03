#include <SFML/Graphics.hpp>
#include <iostream>
#include "../GraphicsObject/GraphicsObject.h"
#include "../GraphicsObject/Character.h"

// push = 0:
/**
 * Check Collision
 * https://www.youtube.com/watch?v=l2iCYCLi6MU
 *
 * push = 0
 *      this object should be completely pushed out of the other
 *
 * push = 1
 *      other object should be completely pushed out of this
 *
 */
bool checkCollision(Character &obj, GraphicsObject &other, float push)
{
    sf::Vector2f thisPosition = obj.getPosition();
    sf::Vector2f thisHalfSize = obj.getSize() / 2.0f;

    sf::Vector2f otherPosition = other.getPosition();
    sf::Vector2f otherHalfSize = other.getSize() / 2.0f;

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;
    float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    std::cout << intersectX << ", " << intersectY << std::endl;


    // collision
    if (intersectX < 0.0f && intersectY < 0.0f)
    {
    std::cout << "HEREEEEEEE" << std::endl;


        push = std::min(std::max(push, 0.0f), 1.0f);

        if ( intersectX > intersectY )
        {
            if ( deltaX > 0.0f )
            {
                obj.move(intersectX * (1.0f - push), 0.0f);
                other.move(-intersectX * push, 0.0f);
            }
            else
            {
                obj.move(-intersectX * (1.0f - push), 0.0f);
                other.move(intersectX * push, 0.0f);
            }
        }
        else
        {
            if ( deltaY > 0.0f )
            {
                obj.move(0.0f, intersectY * (1.0f - push));
                other.move(0.0f, -intersectY * push);
            }
            else
            {
                obj.move(0.0f, -intersectY * (1.0f - push));
                other.move(0.0f, intersectY * push);
            }

        }
    // std::cout << thisPosition.x << ", " << thisPosition.y << std::endl;
    // std::cout << otherPosition.x << ", " << otherPosition.y << std::endl;
        return true;
    }

    return false;
}
