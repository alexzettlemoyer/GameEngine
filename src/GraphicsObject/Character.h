#ifndef CHARACTER_H
#define CHARACTER_H

#include "GraphicsObject.h"

class Character : public GraphicsObject
{
    private:
        sf::Texture characterTexture;
        bool checkBounds(int dir, sf::RenderWindow* window);

    public:
        Character(sf::Vector2f position);
        void left(sf::RenderWindow* window, float dt);
        void up(sf::RenderWindow* window, float dt);
        void right(sf::RenderWindow* window, float dt);
        void down(sf::RenderWindow* window, float dt);
        sf::Vector2f getVelocity();
};

#endif