/*
* Extensão da raylib
*/


#pragma once

#include "raylib.h"

// Raíz quadrada de 2
#define SQRT_2 1.41421356237

typedef struct {
	int x, y;
} Vec2;

Vec2 AddVec2(Vec2, Vec2);
Vec2 SubVec2(Vec2, Vec2);
Vec2 ScaleVec2(Vec2, int);
