/*
* Motor do jogo.
* Pode ser utilizado para fazer vários tipos de jogos e não depende de qualquer arquivo do jogo.
*/


#pragma once

#include "gamelib.h"


typedef struct Scene {
	void (*init)();
	void (*update)();
	void (*draw)();
} Scene;


// Troca a cena atual
void SetScene(Scene scene);
