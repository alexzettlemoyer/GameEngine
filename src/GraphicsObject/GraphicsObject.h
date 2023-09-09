#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include <SFML/Graphics.hpp>

class GraphicsObject : public sf::RectangleShape
{
    private:

    public:
        GraphicsObject(sf::Vector2f size, sf::Vector2f position);
        enum CollisionType { STOP_MOVEMENT, ERASE, PUSH, CHAR };
        CollisionType collisionType;
        // void blockMove();
        sf::Vector2f velocity;
        sf::Vector2f getPosition();
        sf::Vector2f getSize();
        sf::Vector2f getVelocity();
        void left();
        void up();
        void right();
        void down();
        void move(float x, float y);

    protected:
        // CollisionType collisionType;
        // sf::Vector2f velocity;
        bool checkBounds(int dir);
        void updateMovement();
        void blockMove();
        bool loadTexture(sf::Texture& texture, const std::string& image);
};

#endif