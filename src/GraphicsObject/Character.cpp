#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <thread>
#include "GraphicsObject.h"
#include "Character.h"
#include "../Movement/Collider.hpp"
#include "../Time/Timeline.h"
#include "../GameRunner/ClientGameState.h"
#include "../Events/EventHandler.h"
#include "../Events/Event.h"
#include <iostream>

static const std::string IMG_CHARACTER = "images/girl.png";
static const sf::Vector2f SIZE_CHARACTER = sf::Vector2f(116.f, 256.f);
static sf::Vector2u wSize = sf::Vector2u(1000, 800);

static const float displacement = .15f;         // .085f
static const float FRICTION = 3.2f;

sf::Vector2f initialPosition;

// Constructor
Character::Character(sf::Vector2f position, int idNum, std::shared_ptr<Timeline> timeline, std::shared_ptr<SpawnPoint> spawnPoint)
    : GraphicsObject(SIZE_CHARACTER, position, false, idNum, timeline)
{
    characterTexture = std::make_shared<sf::Texture>();
    if (!loadTexture(*characterTexture, IMG_CHARACTER.c_str()))
        {}          // TODO: handle error
    this -> setTexture(characterTexture.get());
    this -> collisionTypeX = CHAR;
    this -> collisionTypeY = CHAR;
    this -> spawnPoint = spawnPoint;
    this -> respawned = false;

    this -> GRAVITY = 700.f;
    this -> acceleration = -2500.f;
}

sf::Vector2f Character::getVelocity()
{
    return GraphicsObject::getVelocity();
}

void Character::left()
{
    float dt = ClientGameState::getInstance() -> getDt();
    float ticSize = ClientGameState::getInstance() -> getTicSize();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (-displacement / dt) * ticSize;
    }
    updateMovement();
}
void Character::up()
{
    float dt = ClientGameState::getInstance() -> getDt();
    float ticSize = ClientGameState::getInstance() -> getTicSize();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y = (acceleration * dt) * ticSize;
    }
    updateMovement();
}
void Character::right()
{
    float dt = ClientGameState::getInstance() -> getDt();
    float ticSize = ClientGameState::getInstance() -> getTicSize();
    { 
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.x += (displacement / dt) * ticSize;
    }
    updateMovement();
}
void Character::down()
{
    float dt = ClientGameState::getInstance() -> getDt();
    float ticSize = ClientGameState::getInstance() -> getTicSize();
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (displacement / dt) * ticSize;
    }
    updateMovement();
}

std::shared_ptr<GraphicsObject> Character::isGrounded()
{
    //  if the character is on top of any of the platforms
    for (std::shared_ptr<GraphicsObject> const& i : (ClientGameState::getInstance() -> getGraphicsObjects()))
    {
        if (i -> isGround() && isCharacterGrounded(*this, *i))
            return i;
    }
    return nullptr;
}

void Character::updateMovement()
{
    std::shared_ptr<GraphicsObject> ground = isGrounded();
    float dt = ClientGameState::getInstance() -> getDt();
    float ticSize = ClientGameState::getInstance() -> getTicSize();

    if (ground == nullptr)
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        velocity.y += (GRAVITY * dt) * ticSize;
    }
    else 
    {
        std::lock_guard<std::mutex> lock(this->objMutex);

        if ( velocity.y > 0 )   // character is moving down
            velocity.y = 0;     // but they're on a ground -> do not allow

        velocity.x += (ground -> getVelocity().x) * FRICTION;
        velocity.y += (ground -> getVelocity().y) * FRICTION;
        

        // sometimes the character moves along an up/down moving platform and add the platforms' velocity
        // but the rendering is delayed, so it appears that the character is standing below the platform
        // to fix this, keep the characters' velocity > the platforms'
        if ( velocity.y > 1.f )
            velocity.y -= 5.f;

    }
    if (!checkBounds())
        move(velocity.x, velocity.y);

    this -> blockMove();
}

void Character::respawn()
{
    this -> respawned = true;

    this -> spawnPoint = std::make_shared<SpawnPoint>();
    this -> setPosition(spawnPoint -> getPosition());
    this -> velocity = sf::Vector2f(0.f, 0.f);
}

bool Character::wasRespawned()
{
    if ( respawned )
    {
        respawned = false;
        return true;
    }
    else
        return false;
}

bool Character::checkBounds()
{
    // window size = 1000x, 800y
    sf::Vector2f position = sf::RectangleShape::getPosition();
    sf::Rect<float> thisRect = (*this).getGlobalBounds();

    // check up
    if ( thisRect.top < 0.f )
    {
        std::lock_guard<std::mutex> lock(this->objMutex);
        this -> setPosition(position.x, 0.1f);
        return false;
    }

    // check if we're beyond any side boundaries
    for ( auto& i : ClientGameState::getInstance() -> getSideBoundaries() )
    {
        if ( checkCollision( *this, *(i.get()), false))
        {
            if ( i -> getDirection() == SideBoundary::RIGHT )
            {
                std::lock_guard<std::mutex> lock(this->objMutex);
                this -> setPosition(i -> getPosition().x - 0.1f - thisRect.width, position.y);
            }

            if ( i -> getDirection() == SideBoundary::LEFT )
            {
                std::lock_guard<std::mutex> lock(this->objMutex);
                this -> setPosition(0.1f, position.y);
            }
        }
    }
        // check if we're colliding with the death zone
    if (checkCollision(*this, *(ClientGameState::getInstance() -> getDeathZone()), false))
    {
        std::shared_ptr<Event> e = std::make_shared<Event>(Event::C_DEATH, ClientGameState::getInstance() -> getTimeStamp());
        e -> addCharacterVariant(this);

        e -> addMetaData("Character " + std::to_string(this -> identifier()) + " death");
        EventHandler::getInstance() -> onEvent(e);
        return false;
    }

    return false;
}

void Character::setGravity(float newGravity)
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    this -> GRAVITY = newGravity;
}

float Character::getGravity()
{
    return GRAVITY;
}

void Character::setAcceleration(float newAcceleration)
{
    std::lock_guard<std::mutex> lock(this->objMutex);
    this -> acceleration = newAcceleration;
}

float Character::getAcceleration()
{
    return acceleration;
}

v8::Local<v8::Object> Character::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("gravity", getCharGravity, setCharGravity));
    v.push_back(v8helpers::ParamContainer("acceleration", getCharAcceleration, setCharAcceleration));

	return v8helpers::exposeToV8("character", this, v, isolate, context, context_name);
}

void Character::setCharGravity(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    float new_gravity_value = value->NumberValue(isolate->GetCurrentContext()).FromMaybe(0);
    Character* obj = static_cast<Character*>(ptr);

    obj->setGravity(new_gravity_value);
}

void Character::getCharGravity(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    Character* obj = static_cast<Character*>(ptr);
    float current_gravity_value = obj->getGravity();

	info.GetReturnValue().Set(current_gravity_value);
}

void Character::setCharAcceleration(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    float new_acceleration_value = value->NumberValue(isolate->GetCurrentContext()).FromMaybe(0);
    Character* obj = static_cast<Character*>(ptr);

    obj->setAcceleration(new_acceleration_value);
}

void Character::getCharAcceleration(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    Character* obj = static_cast<Character*>(ptr);
    float current_acceleration_value = obj->getAcceleration();

	info.GetReturnValue().Set(current_acceleration_value);
}

