#ifndef CHARACTER_H
#define CHARACTER_H

#include "GraphicsObject.h"

class Character : public virtual sf::Sprite
{
    private:
        sf::Texture characterTexture;
        bool checkBounds(int dir, sf::RenderWindow* window);
        bool loadTexture(sf::Texture& texture, const std::string& image);

    public:
        Character();
        void left(sf::RenderWindow* window);
        void up(sf::RenderWindow* window);
        void right(sf::RenderWindow* window);
        void down(sf::RenderWindow* window);
        sf::Vector2f getPosition();
        sf::Vector2f getSize();
        void move(float x, float y);

};

#endif