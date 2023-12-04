#ifndef ITEM_H
#define ITEM_H

#include "GraphicsObject.h"
#include "../Time/Timeline.h"

class Item : public GraphicsObject
{
    private:
        std::shared_ptr<sf::Texture> moneyTexture;
        
    public:
        Item(const sf::Vector2f& position, int idNum, std::shared_ptr<Timeline> timeline);
        int getType() const override { return GraphicsObject::ITEM_TYPE; }
};

#endif