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

// Adicionar dois Vec2
Vec2 AddVec2(Vec2, Vec2);

// Subtrair o segundo Vec2 do primeiro
Vec2 SubVec2(Vec2, Vec2);

// Multiplicar cada componente pela escalar
Vec2 ScaleVec2(Vec2, int);

// Calcular a magnitude do vetor
int Vec2Magnitude(Vec2);
