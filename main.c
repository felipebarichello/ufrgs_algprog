/*
* Inicialização da janela e incorporação do programa
*/


#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


int main() {
	srand(time(NULL));

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pac-Mine");
	// ToggleFullscreen();

	Init(); // Implementado por game.c

	while (!WindowShouldClose()) {
		HandleInput();
		Update(); // Implementado por game.c

		BeginDrawing();
		Draw(); // Implementado por game.c
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
