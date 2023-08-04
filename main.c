/*
* Inicializa��o da janela e incorpora��o do programa
*/


#include <stdlib.h>
#include <time.h>
#include "gamelib.h"
#include "level.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

int main() {
	srand(time(NULL));

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pac-Mine");

	// Menu_Init();
	Level_Init(); // Implementado por game.c

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_F11)) {
			ToggleFullscreen();
		}

		Level_Update(); // Implementado por game.c

		BeginDrawing();
		Level_Draw(); // Implementado por game.c
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
