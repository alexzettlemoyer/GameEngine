#include <SFML/Graphics.hpp>
#include "movement.hpp"
#include "../graphics/graphicsObjects.hpp"

// static const float MAX_Y = 50f;
static const float GRAVITY = 5;

// obj to check the bounds for
// dir 0=left, 1=up, 2=right, 3=down
// the window to check bounds in
bool checkBounds(sf::Sprite s, int dir, sf::RenderWindow* window)
{
    sf::Vector2u wSize = (*window).getSize();
    sf::Vector2f position = s.getPosition();
    sf::Rect<float> spriteRect = s.getGlobalBounds();
    
    switch ( dir )
    {
        case 0:     // check left
            if ( position.x - 1 <= 0 )
                return false;
            break;
        case 1:     // check up
           //  std::cout << position.y - 1 << std::endl;
            if ( position.y - 1 <= 0 )
                return false;
            break;
        case 2:     // check right
            if ( position.x + spriteRect.width + 1 >= wSize.x )
                return false;
            break;
        case 3:     // check down
            if ( position.y + spriteRect.height + 1 >= wSize.y )
                return false;
            break;
        default:
            return true;
            break;
    }

    li

    if ( spriteRect.intersects(item.getGlobalBounds()) || spriteRect.intersects(platform2.getGlobalBounds()))
        return false;

    return true;
}

bool checkCollision()
{
    sf::Rect<float> spriteRect = character.getGlobalBounds();
    if ( spriteRect.intersects(item.getGlobalBounds()) || spriteRect.intersects(platform2.getGlobalBounds()))
        return false;
    return true;
}



float x = 0;
float y = 0;
float xVelocity = 0;
float yVelocity = 0;
float xAcceleration = 0;
float yAcceleration = 0;

void updateMovement()
{
    yVelocity += GRAVITY;

    xVelocity += xAcceleration;
    yVelocity += yAcceleration;

    x += xVelocity;
    y += yVelocity;

    if ( !checkCollision())
        character.setPosition(x, y);
}


// void jump(sf::RenderWindow* window)      // JUMP DRAFT
// {
//     // set the jump velocity
//     yVelocity = -10;
//     updateMovement();

//     // sf::Vector2f velocity(2.f, 5.f);

//     // // jump up
//     // if (checkBounds(character, 1, window))
//     //     character.move(0.f, -3.f);

// }

// void right( sf::RenderWindow* window )       // JUMP DRAFT
// {
//     if ( checkBounds(character, 2, window) )
//         xVelocity = 5;
//     updateMovement();
// }
