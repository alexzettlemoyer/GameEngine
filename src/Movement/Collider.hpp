#ifndef COLLIDER_H
#define COLLIDER_H

#include "../GraphicsObject/Character.h"
#include "../GraphicsObject/GraphicsObject.h"

bool checkCollision(Character &obj, GraphicsObject &other, float push);

#endif