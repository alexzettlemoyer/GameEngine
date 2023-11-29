#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include <SFML/Graphics.hpp>
#include <mutex>
#include <memory>
#include "../Time/Timeline.h"
#include "GraphicsObject.h"
#include <v8/v8.h>
#include "../Scripting/v8helpers.h"

class GraphicsObject : public sf::RectangleShape
{
    typedef void (*MovementFunction)(GraphicsObject&);

    public:
        GraphicsObject(sf::Vector2f size, sf::Vector2f position, bool isGround, int idNum, std::shared_ptr<Timeline> timeline);
        ~GraphicsObject();
        enum CollisionType { STOP_MOVEMENT, ERASE, PUSH, CHAR, DEATH, SCROLL, NONE };

        // GraphicsObject types
        static const int CHARACTER_TYPE = 1;
        static const int PLATFORM_TYPE = 2;
        static const int ITEM_TYPE = 3;
        static const int DEATHZONE_TYPE = 4;
        static const int SIDE_BOUNDARY_TYPE = 5;
        static const int SPAWN_POINT_TYPE = 6;

        // some properties
        CollisionType collisionTypeX;
        CollisionType collisionTypeY;
        sf::Vector2f velocity;
        sf::Vector2f previousVelocity;
        sf::Vector2f getOriginalPosition();
        sf::Vector2f getPosition();
        sf::Vector2f getSize();
        sf::Vector2f getVelocity();

        // movement functions
        void left();
        void up();
        void right();
        void down();
        void move(float x, float y);
        std::mutex objMutex;

        // attribute getters
        bool isGround();
        int identifier();
        virtual int getType() const = 0;

        // movement pattern functions
        void setMovementFunction(MovementFunction func);
        MovementFunction getMovementFunction();

        std::shared_ptr<Timeline> getTimeline();

        std::string guid;

        /**
		 * This function will make this class instance accessible to scripts in
		 * the given context. 
		 *
		 * IMPORTANT: Please read this definition of this function in
		 * GameObject.cpp. The helper function I've provided expects certain
		 * parameters which you must use in order to take advance of this
		 * convinience. 
		 */
		v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");

    protected:
        bool ground;
        int id;
        sf::Vector2f originalPosition;
        bool checkBounds();
        void updateMovement();
        void blockMove();
        bool loadTexture(sf::Texture& texture, const std::string& image);
        std::shared_ptr<Timeline> timeline;
        MovementFunction movementFunction = nullptr;

        /**
		 * NOTE: These "Accessors" have to be **static**
		 *
		 * You will need to implement a setter and getter for every class
		 * member variable you want accessible to javascript.
		 */
		static void setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
		static void getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
		static void setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
		static void getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
		static void setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
		static void getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
};

#endif