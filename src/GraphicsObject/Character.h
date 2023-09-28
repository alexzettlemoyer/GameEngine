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
        Character(sf::Vector2f position, int idNum);
        void updateMovement();
        void left();
        void up();
        void right();
        void down();
        void addWindowReference(sf::RenderWindow* window);
        sf::Vector2f getVelocity();
        int getType() const override { return GraphicsObject::CHARACTER_TYPE; }
};

#endif