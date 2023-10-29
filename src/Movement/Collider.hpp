#ifndef COLLIDER_H
#define COLLIDER_H

#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/GraphicsObject.h"

bool isCharacterGrounded(Character &character, GraphicsObject &ground);
bool collisionResponse(GraphicsObject &obj, GraphicsObject &obj2, int dir);
bool checkCollision(GraphicsObject &obj, GraphicsObject &other, bool withResponse);

#endif