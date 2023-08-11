#pragma once

#include "gamelib.h"
#include "level_consts.h"


#define millis2frames(millis) millis * FPS / 1000
#define matrix2screen(mpos) AddVec2(ScaleVec2(mpos, unit_length), level_offset)
#define screen2matrix(spos) DivideVector2(SubVector2(spos, Vector2FromVec2(level_offset)), unit_length)


typedef struct {
	// Posição do inimigo da matriz
	Vec2 position;

	// Para que direção o inimigo está virado
	// 1, 0 ou -1 cada componente
	Vec2 direction;

	// Tempo para o inimigo poder se mover de novo
	int move_cooldown;
} Enemy;

typedef struct {
	Enemy enemy;
	char active; // Booleana para o estado do inimigo
} PooledEnemy;

typedef struct {
	// "Double kill", "triple kill", etc.
	// "First blood" é considerado um som de combo
	Sound combo[COMBO_SOUNDS];

	Sound gunshot;   // Som do tiro
	Sound empty_gun; // Som do tiro quando está em cooldown
	Sound kill;     // Som de morte do inimigo
} LevelSounds;

typedef enum {
	T_EMPTY    = 0,   // Ausência de tile. Todos os outros códigos inválidos também são considerados vazios.

	// Entidades
	T_PLAYER  = 'J', // O Jogador
	T_ENEMY   = 'T', // Cada inimigo

	// Sólidos
	T_WALL    = '#', // Paredes
	T_BURIED  = 'S', // Áreas soterradas
	
	// Consumíveis
	T_EMERALD = 'E', // Esmeralda
	T_GOLD    = 'O', // Ouro
	T_POWERUP = 'A', // Power-up
} Tile;
