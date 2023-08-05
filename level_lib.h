#pragma once

#include "gamelib.h"

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

typedef enum {
	T_EMPTY    = 0,   // Ausência de tile. Todos os outros códigos inválidos também são considerados vazios.

	// Entidades
	T_PLAYER  = 'J', // O Jogador
	T_ENEMY   = 'T', // Cada inimigo

	// Sólidos
	T_WALL    = '#', // Paredes          [Sólido, indestrutível]
	T_BURIED  = 'S', // Áreas soterradas [Sólido, destrutível]
	
	// Consumíveis
	T_EMERALD = 'E', // Esmeralda
	T_GOLD    = 'O', // Ouro
	T_POWERUP = 'A', // Power-up
} Tile;
