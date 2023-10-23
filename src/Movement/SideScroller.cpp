#include "SideScroller.h"
#include "../GameRunner/GameState.h"

SideScroller* SideScroller::instancePtr = nullptr;

static const float SCROLL_SPEED = 5.f;

SideScroller::SideScroller()
{ }

SideScroller* SideScroller::getInstance()
{
    if (instancePtr == NULL)
        { instancePtr = new SideScroller(); }
    return instancePtr;
}

float SideScroller::getSideScrollSpeed()
{
    return sideScrollSpeed;
}

float SideScroller::getSideScrollDistance()
{
    return totalScrollDistance;
}

void SideScroller::scrollWindow(int direction)
{
    // first edit the side scroll speed to in the direction
    if (direction == SideBoundary::RIGHT && totalScrollDistance < MAX_POSITION)
        sideScrollSpeed = SCROLL_SPEED;

    if (direction == SideBoundary::LEFT && totalScrollDistance > MIN_POSITION)
        sideScrollSpeed = -SCROLL_SPEED;

    // track the total distance scrolled
    totalScrollDistance += sideScrollSpeed;

    // update all graphicsObjects' movement in the direction of the side scroll speed
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects())
    {
        if ( i -> getType() != GraphicsObject::CHARACTER_TYPE ) 
        {
            i -> move( -sideScrollSpeed, 0.0f );
        }
    }

    // reset the side scroll speed to 0
    sideScrollSpeed = 0.f;
}

void SideScroller::reset()
{
    for (std::shared_ptr<GraphicsObject> const& i : GameState::getInstance() -> getGraphicsObjects())
    {
        i -> setPosition( i -> getOriginalPosition() );
    }
    totalScrollDistance = 0.f;
    sideScrollSpeed = 0.f;
}