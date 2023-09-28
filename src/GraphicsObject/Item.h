#ifndef ITEM_H
#define ITEM_H

#include "GraphicsObject.h"

class Item : public GraphicsObject
{
    private:
        sf::Texture moneyTexture;
        
    public:
        Item(const sf::Vector2f& position, int idNum);
        int getType() const override { return GraphicsObject::ITEM_TYPE; }
};

#endif