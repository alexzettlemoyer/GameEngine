#ifndef COLLIDER_H
#define COLLIDER_H

#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/Platform.h"
#include "../GraphicsObject/GraphicsObject.h"

bool isCharacterGrounded(Character &character, Platform &platform);
bool collisionResponse(GraphicsObject &obj, int dir);
bool checkCollision(GraphicsObject &obj, GraphicsObject &other, float push, float mvmntDir = -1);

#endif