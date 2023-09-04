#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include <SFML/Graphics.hpp>

class GraphicsObject : public sf::RectangleShape
{
    public:
        GraphicsObject();
        bool loadTexture(sf::Texture& texture, const std::string& image);
        sf::Vector2f getPosition();
        sf::Vector2f getSize();
        void move(float x, float y);

    private:
//        sf::Vector2f velocity;
};

#endif