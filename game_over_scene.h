/*
* Cena: A tela de game over
*/

#pragma once

#include "engine.h"

Scene GameOver_Scene();

typedef enum Ending {
	VICTORY,
	DEFEAT
} Ending;

typedef struct GameOver_Args {
	Ending ending;
	int score;
} GameOver_Args;
