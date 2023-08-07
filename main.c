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
	InitAudioDevice();

	// Menu_Init();
	Level_Init(); // Implementado por game.c

	while (!WindowShouldClose()) {
		Level_Update(); // Implementado por game.c

		BeginDrawing();
			Level_Draw(); // Implementado por game.c
		EndDrawing();
	}

	CloseAudioDevice();
	CloseWindow();

	return 0;
}
