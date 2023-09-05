#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include "../graphics/draw.hpp"

int mvmntDirCW = 0;                     // movement direction for the clockwise movement
sf::Vector2f originalPositionCW;        // original position for the clockwise movement

int mvmntDirLR = 0;                     // movement direction for the left/right movement
sf::Vector2f originalPositionLR;        // original position for the left/right movement

const float CHANGE_DIFF_CW = 100.f;     // movement amount for clockwise
const float CHANGE_DIFF_LR = 200.f;     // movement amount for left/right

const float MOVE_DIST = 2.0f;           // movement amount for individual steps

bool moveLeft(GraphicsObject& obj)
{
    obj.move(-MOVE_DIST, 0.f);
    return true;
}
bool moveUp(GraphicsObject& obj)
{
    obj.move(0.f, -MOVE_DIST);     // move up
    return true;
}
bool moveRight(GraphicsObject& obj)
{
    obj.move(MOVE_DIST, 0.f);     // move right
    return true;
}
bool moveDown(GraphicsObject& obj)
{
    obj.move(0.f, MOVE_DIST);     // move down
    return true;
}

/**
 * Clockwise movement
 * left     0
 * up       1
 * right    2
 * down     3
 * 
 * @param &obj the address of the object to move
 * @return the boolean result of the movement
 */
bool movementClockwise(GraphicsObject& obj)
{
    // if the first time, get the original position
    if ( originalPositionCW == sf::Vector2f(0.f, 0.f) )
        originalPositionCW = obj.getPosition();

    // get the difference to the original position
    float xDiff = std::abs(obj.getPosition().x - originalPositionCW.x);
    float yDiff = std::abs(obj.getPosition().y - originalPositionCW.y);

    if ( mvmntDirCW == 0 && xDiff <= CHANGE_DIFF_CW )  // left movement
    {
        if (!moveLeft(obj))
            return false;
        if ( xDiff >= CHANGE_DIFF_CW )
            mvmntDirCW = 1;
    }
    if ( mvmntDirCW == 1 && yDiff <= CHANGE_DIFF_CW )  // up movement
    {
        if (!moveUp(obj))
            return false;
        if ( yDiff >= CHANGE_DIFF_CW )
            mvmntDirCW = 2;
    }
    if ( mvmntDirCW == 2 && xDiff >= 0 )            // right movement
    {
        if (!moveRight(obj))
            return false;
         if ( xDiff <= 0 )
            mvmntDirCW = 3;
    }
    if ( mvmntDirCW == 3 && yDiff >= 0 )            // down movement
    {
        if (!moveDown(obj))
            return false;
        if ( yDiff <= 0 )
            mvmntDirCW = 0;
    }

    checkCollision(*character, obj, 1.0f, mvmntDirCW);
    return true;
}


// Movement pattern:
// - left   0
// - right  2
bool movementLeftRight(GraphicsObject& obj)
{
    // if the first time, get the original position
    if ( originalPositionLR == sf::Vector2f(0.f, 0.f) )
        originalPositionLR = obj.getPosition();

    float xDiff = std::abs(obj.getPosition().x - originalPositionLR.x);

    if ( mvmntDirLR == 0 && xDiff <= CHANGE_DIFF_LR )  // left movement
    {
        if (!moveLeft(obj))
            return false;
        if ( xDiff >= CHANGE_DIFF_LR )
            mvmntDirLR = 2;
    }
    if ( mvmntDirLR == 2 && xDiff >= 0 )            // right movement
    {
        if (!moveRight(obj))
            return false;
        if ( xDiff <= 0 )
            mvmntDirLR = 0;
    }

    checkCollision(*character, obj, 1.0f, mvmntDirLR);
    return true;
}
