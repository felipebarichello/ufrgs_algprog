#pragma once

#include "gamelib.h"

typedef struct {
	Vec2 position;
} Player;

typedef struct {
	// Posi��o do inimigo da matriz
	Vec2 position;

	// Para que dire��o o inimigo est� virado
	// 1, 0 ou -1 cada componente
	Vec2 direction;

	// Tempo para o inimigo poder se mover de novo
	int move_cooldown;
} Enemy;
