#ifndef PLATFORM_H
#define PLATFORM_H

#include "GraphicsObject.h"
#include "../Time/Timeline.h"

class Platform : public GraphicsObject
{
    private:
        sf::Texture platformTexture;
    
    public: 
        Platform(const sf::Vector2f& position, int idNum, Timeline* timeline, int colorNum = 0);
        int getType() const override { return GraphicsObject::PLATFORM_TYPE; }
};

#endif