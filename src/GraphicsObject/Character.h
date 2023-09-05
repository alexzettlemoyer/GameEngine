#ifndef CHARACTER_H
#define CHARACTER_H

#include "GraphicsObject.h"

class Character : public GraphicsObject
{
    private:
        sf::Texture characterTexture;
        // sf::Vector2f velocity;
        bool checkBounds(int dir, sf::RenderWindow* window);
        // void updateMovement();
        // void blockMove();

    public:
        Character(sf::Vector2f position);
        void left(sf::RenderWindow* window, float dt);
        void up(sf::RenderWindow* window, float dt);
        void right(sf::RenderWindow* window, float dt);
        void down(sf::RenderWindow* window, float dt);
        sf::Vector2f getVelocity();
        // sf::Vector2f getPosition();
        // sf::Vector2f getSize();
        // void move(float x, float y);

};

#endif