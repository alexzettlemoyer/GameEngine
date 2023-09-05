#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include <SFML/Graphics.hpp>

class GraphicsObject : public sf::RectangleShape
{
    private:
        sf::Vector2f velocity;
        bool checkBounds(int dir, sf::RenderWindow* window);
        void updateMovement();
        void blockMove();

    public:
        GraphicsObject(sf::Vector2f size, sf::Vector2f position);
        bool loadTexture(sf::Texture& texture, const std::string& image);
        sf::Vector2f getPosition();
        sf::Vector2f getSize();
        sf::Vector2f getVelocity();
        void left(sf::RenderWindow* window, float dt);
        void up(sf::RenderWindow* window, float dt);
        void right(sf::RenderWindow* window, float dt);
        void down(sf::RenderWindow* window, float dt);
        void move(float x, float y);
};

#endif