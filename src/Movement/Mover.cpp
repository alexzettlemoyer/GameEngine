#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include "../Time/Timeline.h"
#include <iostream>

int mvmntDirCW = 0;                     // movement direction for the clockwise movement
sf::Vector2f originalPositionCW;        // original position for the clockwise movement

int mvmntDirLR = 0;                     // movement direction for the left/right movement
sf::Vector2f originalPositionLR;        // original position for the left/right movement

int mvmntDirUPDOWN = 0;                 // movement direction for the up/right movement
sf::Vector2f originalPositionUPDOWN;    // original position for the up/down movement

const float CHANGE_DIFF_CW = 100.f;     // movement amount for clockwise
const float CHANGE_DIFF_LR = 200.f;     // movement amount for left/right
const float CHANGE_DIFF_UPDOWN = 200.f;

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
    originalPositionCW = obj.getOriginalPosition();

    // get the difference to the original position
    float xDiff = originalPositionCW.x - obj.getPosition().x;
    float yDiff = originalPositionCW.y - obj.getPosition().y;

    if ( mvmntDirCW == 0 )          // left movement
    {
        obj.left();
        if ( xDiff >= CHANGE_DIFF_CW ) 
            mvmntDirCW = 1;
    }
    if ( mvmntDirCW == 1 )          // up movement
    {
        obj.up();
        if ( yDiff >= CHANGE_DIFF_CW )
            mvmntDirCW = 2;
    }
    if ( mvmntDirCW == 2 )            // right movement
    {
        obj.right();
        if ( xDiff <= 0 )
            mvmntDirCW = 3;
    }
    if ( mvmntDirCW == 3 )            // down movement
    {
        obj.down();
        if ( yDiff <= 0 )
            mvmntDirCW = 0;
    }
    return true;
}


// Movement pattern:
// - left   0
// - right  2
bool movementLeftRight(GraphicsObject& obj)
{  
    // if the first time, get the original position
    originalPositionLR = obj.getOriginalPosition();
    float xDiff = originalPositionLR.x - obj.getPosition().x;

    if ( mvmntDirLR == 0  )  // left movement
    {
        obj.left();
        if ( xDiff >= CHANGE_DIFF_LR )
            mvmntDirLR = 1;
    }
    if ( mvmntDirLR == 1  )  // right movement
    {
        obj.right();
        if ( xDiff <= 0 )
            mvmntDirLR = 0;
    }
    return true;
}

// Movement pattern:
// - down   0
// - up     1
bool movementUpDown(GraphicsObject& obj)
{
    originalPositionUPDOWN = obj.getOriginalPosition();
    float yDiff = obj.getPosition().y - originalPositionUPDOWN.y;

    if ( mvmntDirUPDOWN == 0 )  // down movement
    {
        obj.down();
        if ( yDiff >= CHANGE_DIFF_UPDOWN )
            mvmntDirUPDOWN = 1;
    }
    if ( mvmntDirUPDOWN == 1 )   // up movement
    {
        obj.up();
        if ( yDiff <= 0 )
            mvmntDirUPDOWN = 0;
    }
    return true;
}