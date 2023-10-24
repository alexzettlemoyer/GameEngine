#include "SideScroller.h"
#include "../GameRunner/GameState.h"
#include <iostream>

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

bool SideScroller::checkSideCollision(GraphicsObject* character, GraphicsObject* sideBoundary)
{
    sf::FloatRect characterBounds = character -> getGlobalBounds();
    sf::FloatRect sideBounds = sideBoundary -> getGlobalBounds();
    sideBounds.left += totalScrollDistance;


    int collisionDirection;
    sf::FloatRect intersection;
    if ( characterBounds.intersects(sideBounds, intersection))
    {
        if ( intersection.width < intersection.height  )    // x collision
        {
            std::cout << "Colliding in Side Scroller" << std::endl;
            // scroll the window
            
            scrollWindow(dynamic_cast<SideBoundary*>(sideBoundary) -> getDirection());
        }
        else { }    // y collisions - do nothing
        return true;
    }
    return false;
}

void SideScroller::scrollWindow(int direction)
{
    // first edit the side scroll speed to in the direction
    std::cout << "DIRECTION \t" << direction << std::endl;

    // Right side boundary collision:
    if (direction == SideBoundary::RIGHT && totalScrollDistance < MAX_POSITION)
        totalScrollDistance += SCROLL_SPEED;

    if (direction == SideBoundary::LEFT && totalScrollDistance > MIN_POSITION)
        totalScrollDistance -= SCROLL_SPEED;

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