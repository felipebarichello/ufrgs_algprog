#pragma once

// Raíz quadrada de 2
#define SQRT_2 1.41421356237

typedef struct {
	int x, y;
} Vec2;

typedef struct {
	Vec2 position;
} Player;

typedef struct {
	// Posição do inimigo da matriz
	Vec2 position;

	// Para que direção o inimigo está virado
	// 1, 0 ou -1 cada componente
	Vec2 direction;

	// Tempo para o inimigo poder se mover de novo
	int move_cooldown;
} Enemy;

Vec2 AddVec2(Vec2, Vec2);
Vec2 SubVec2(Vec2, Vec2);
Vec2 ScaleVec2(Vec2, int);
