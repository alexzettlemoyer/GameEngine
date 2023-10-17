#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include <SFML/Graphics.hpp>
#include <mutex>
#include "../Time/Timeline.h"


class GraphicsObject : public sf::RectangleShape
{
    public:
        GraphicsObject(sf::Vector2f size, sf::Vector2f position, bool isGround, int idNum, Timeline* timeline);
        enum CollisionType { STOP_MOVEMENT, ERASE, PUSH, CHAR, DEATH, SCROLL, NONE };
        static const int CHARACTER_TYPE = 1;
        static const int PLATFORM_TYPE = 2;
        static const int ITEM_TYPE = 3;
        static const int DEATHZONE_TYPE = 4;
        static const int SIDE_BOUNDARY_TYPE = 5;
        CollisionType collisionTypeX;
        CollisionType collisionTypeY;
        sf::Vector2f velocity;
        sf::Vector2f getPosition();
        sf::Vector2f getSize();
        sf::Vector2f getVelocity();
        void left();
        void up();
        void right();
        void down();
        void move(float x, float y);
        std::mutex objMutex;
        bool isGround();
        int identifier();
        virtual int getType() const = 0;


    protected:
        bool ground;
        int id;
        bool checkBounds();
        void updateMovement();
        void blockMove();
        bool loadTexture(sf::Texture& texture, const std::string& image);
        Timeline *timeline;
};

#endif