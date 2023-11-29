#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <mutex>
#include "GraphicsObject.h"
#include "../Movement/Collider.hpp"
#include "../Time/Timeline.h"
#include "../GameRunner/ServerGameState.h"
#include <iostream>

/** Definitions of static class members */
std::vector<GraphicsObject*> GraphicsObject::game_objects;

const float displacement = .025f;

GraphicsObject::GraphicsObject(sf::Vector2f size, sf::Vector2f position, bool isGround, int idNum, std::shared_ptr<Timeline> timeline)
{
    this -> setSize(size);
    this -> setPosition(position);
    originalPosition = position;
    ground = isGround;

    velocity.x = 0.f;
    velocity.y = 0.f;

    previousVelocity = sf::Vector2f(0.f, 0.f);
    id = idNum;

    this -> timeline = timeline;

    guid = "gameobject" + std::to_string(id);
}

GraphicsObject::~GraphicsObject()
{
	context->Reset();
}

/**
 * IMPORTANT: Pay close attention to the definition of the std::vector in this
 * example implementation. The v8helpers::expostToV8 will assume you have
 * instantiated this exact type of vector and passed it in. If you don't the
 * helper function will not work. 
 */
v8::Local<v8::Object> GraphicsObject::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    if (!isolate || isolate->IsDead() || isolate->IsExecutionTerminating())
        std::cout << "Isolate is inactive or terminated!" << std::endl;

    if (context.IsEmpty())
        std::cout << "Invalid or empty context!" << std::endl;


	std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
	v.push_back(v8helpers::ParamContainer("x", getGameObjectX, setGameObjectX));
	v.push_back(v8helpers::ParamContainer("y", getGameObjectY, setGameObjectY));
	v.push_back(v8helpers::ParamContainer("guid", getGameObjectGUID, setGameObjectGUID));

	return v8helpers::exposeToV8(guid, this, v, isolate, context, context_name);
}

/**
 * Implementations of static setter and getter functions
 *
 * IMPORTANT: These setter and getter functions will set and get values of v8
 * callback data structures. Note their return type is void regardless of
 * whether they are setter or getter. 
 *
 * Also keep in mind that the function signature must match this exactly in
 * order for v8 to accept these functions. 
 */ 

void GraphicsObject::setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    int32_t xValue = value->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);
    GraphicsObject* obj = static_cast<GraphicsObject*>(ptr);
    sf::Vector2f currentPosition = obj->getPosition();
    obj->setPosition(xValue, currentPosition.y);
}

void GraphicsObject::getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    GraphicsObject* obj = static_cast<GraphicsObject*>(ptr);
    sf::Vector2f currentPosition = obj->getPosition();

	int x_val = int(currentPosition.x);
	info.GetReturnValue().Set(x_val);
}

void GraphicsObject::setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    int32_t yValue = value->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);

    GraphicsObject* obj = static_cast<GraphicsObject*>(ptr);
    sf::Vector2f currentPosition = obj->getPosition();
    obj->setPosition(currentPosition.x, yValue);
}

void GraphicsObject::getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    GraphicsObject* obj = static_cast<GraphicsObject*>(ptr);
    sf::Vector2f currentPosition = obj->getPosition();

	int y_val = int(currentPosition.y);
	info.GetReturnValue().Set(y_val);
}

void GraphicsObject::setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString(info.GetIsolate()));
	static_cast<GraphicsObject*>(ptr)->guid = *utf8_str;
}

void GraphicsObject::getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	std::string guid = static_cast<GraphicsObject*>(ptr)->guid;
	v8::Local<v8::String> v8_guid = v8::String::NewFromUtf8(info.GetIsolate(), guid.c_str(), v8::String::kNormalString);
	info.GetReturnValue().Set(v8_guid);
}

void GraphicsObject::setMovementFunction(MovementFunction func) {
    movementFunction = func;
}

GraphicsObject::MovementFunction GraphicsObject::getMovementFunction()
{
    return movementFunction;
}

bool GraphicsObject::loadTexture(sf::Texture& texture, const std::string& image)
{
    if (!texture.loadFromFile(image))
        return false;
    return true;
}

int GraphicsObject::identifier()
{
    return id;
}

sf::Vector2f GraphicsObject::getPosition()
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    return sf::RectangleShape::getPosition();
}

sf::Vector2f GraphicsObject::getOriginalPosition()
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    return originalPosition;
}

sf::Vector2f GraphicsObject::getSize()
{
    return sf::RectangleShape::getSize();
}

void GraphicsObject::move(float x, float y)
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    sf::RectangleShape::move(x, y);
}

sf::Vector2f GraphicsObject::getVelocity()
{    
    if (velocity == sf::Vector2f(0.f, 0.f) && previousVelocity != sf::Vector2f(0.f, 0.f))
        return previousVelocity;
    return sf::Vector2f(velocity.x, velocity.y);
}

void GraphicsObject::left()
{
    float dt = timeline -> getDt();
    if (dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (-displacement / dt) * timeline -> getTicSize();
    }
    (*this).checkBounds();
    updateMovement();
}

void GraphicsObject::up()
{
    float dt = timeline -> getDt();
    if (dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (-displacement / dt) * timeline -> getTicSize();
    }
    (*this).checkBounds();
    updateMovement();
}
void GraphicsObject::right()
{
    float dt = timeline -> getDt();
    if (dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (displacement / dt) * timeline -> getTicSize();
    }
    (*this).checkBounds();
    updateMovement();
}
void GraphicsObject::down()
{
    float dt = timeline -> getDt();
    if (dt != 0)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (displacement / dt) * timeline -> getTicSize();
    }
    (*this).checkBounds();
    updateMovement();
}

void GraphicsObject::blockMove()
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    this -> previousVelocity = sf::Vector2f(velocity.x, velocity.y);
    velocity.x = 0.f;
    velocity.y = 0.f;
}

void GraphicsObject::updateMovement()
{
    this -> move(velocity.x, velocity.y);
    this -> blockMove();
}

bool GraphicsObject::checkBounds()
{
    for (Character* i : ServerGameState::getInstance() -> getCharacters())
    {
        if (checkCollision(*i, *this, true))
            return false;
    }
    return true;
}

bool GraphicsObject::isGround()
{
    return ground;
}

std::shared_ptr<Timeline> GraphicsObject::getTimeline()
{
    return timeline;
}
