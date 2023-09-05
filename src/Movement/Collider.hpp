#ifndef COLLIDER_H
#define COLLIDER_H

// #include "../GraphicsObject/Character.h"
#include "../GraphicsObject/GraphicsObject.h"

bool checkCollision(GraphicsObject &obj, GraphicsObject &other, float push, float mvmntDir = -1);

#endif