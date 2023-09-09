#ifndef CHARACTER_H
#define CHARACTER_H

#include "GraphicsObject.h"

class Character : public GraphicsObject
{
    private:
        sf::Texture characterTexture;
        bool isGrounded();
        bool checkBounds();

    public:
        Character(sf::Vector2f position);
        void updateMovement();
        void left();
        void up();
        void right();
        void down();
        void addWindowReference(sf::RenderWindow* window);
        sf::Vector2f getVelocity();
};

#endif