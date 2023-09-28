#ifndef PLATFORM_H
#define PLATFORM_H

#include "GraphicsObject.h"

class Platform : public GraphicsObject
{
    private:
        sf::Texture platformTexture;
    
    public: 
        Platform(const sf::Vector2f& position, int idNum);
        int getType() const override { return GraphicsObject::PLATFORM_TYPE; }
};

#endif