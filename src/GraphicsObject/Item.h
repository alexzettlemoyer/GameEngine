#ifndef ITEM_H
#define ITEM_H

#include "GraphicsObject.h"

class Item : public GraphicsObject
{
    private:
        sf::Texture moneyTexture;
    public:
        Item(const sf::Vector2f& size, const sf::Vector2f& position);
};

#endif