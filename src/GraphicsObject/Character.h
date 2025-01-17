#ifndef CHARACTER_H
#define CHARACTER_H

#include "GraphicsObject.h"
#include "SpawnPoint.h"
#include "../Time/Timeline.h"
#include <memory>
#include <v8/v8.h>


class Character : public GraphicsObject
{
    private:
        std::shared_ptr<sf::Texture> characterTexture;
        std::shared_ptr<GraphicsObject> isGrounded();
        std::shared_ptr<SpawnPoint> spawnPoint;
        bool checkBounds();
        bool respawned;

        float GRAVITY;
        float acceleration;

        static void setCharGravity(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
		static void getCharGravity(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void setCharAcceleration(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
		static void getCharAcceleration(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    public:
        Character(sf::Vector2f position, int idNum, std::shared_ptr<Timeline> timeline, std::shared_ptr<SpawnPoint> spawnPoint = nullptr);
        void updateMovement();
        void left();
        void up();
        void right();
        void down();
        void respawn();
        bool wasRespawned();
        sf::Vector2f getVelocity();
        int getType() const override { return GraphicsObject::CHARACTER_TYPE; }

        void setGravity(float newGravity);
        float getGravity();

        void setAcceleration(float newAcceleration);
        float getAcceleration();

		v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");
        
};

#endif