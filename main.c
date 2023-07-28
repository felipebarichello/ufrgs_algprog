/*
 * Inicialização da janela e incorporação do programa
 */


#include "raylib.h"
#include "game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GTA VI");

	Init(); // Implementado por game.c

	while (!WindowShouldClose()) {
		Update(); // Implementado por game.c

		BeginDrawing();
		Draw(); // Implementado por game.c
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
