/*
* Cena: O nível do jogo.
*/

#pragma once

#include "engine.h"

Scene Level_Scene();

// Informação que a cena Level precisa para iniciar
typedef struct Level_Args {
    char load_saved_game; // Booleano. Se for 1, carrega um nível. Se for 0, inicia um novo jogo.
} Level_Args;
