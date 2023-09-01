#include "level_scene.h"
#include "level_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include "map_reader.h"
#include "engine.h"
#include "scene.h"
#include "level_consts.h"

scene(Victory)

void Victory_Init(void* _) {

}

void Victory_Update(void (*set_scene)(Scene scene)) {
	if (IsKeyPressed(KEY_N)) {
		Level_Args* level_args = malloc(sizeof(Level_Args));
		level_args->load_saved_game = 0;
		set_scene(Level_Scene(), level_args);
	}

	if (IsKeyPressed(KEY_C)) {
		Level_Args* level_args = malloc(sizeof(Level_Args));
		level_args->load_saved_game = 1;
		set_scene(Level_Scene(), level_args);
	}

	if (IsKeyPressed(KEY_Q)) {
		CloseWindow();
	}
}

void Victory_Draw() {
	ClearBackground(RAYWHITE);
	DrawText("Vit�ria", 175, 100, 3 * FONT_SIZE, GREEN);
	DrawText("(N): Novo Jogo", 300, 250, FONT_SIZE, BLACK);
	DrawText("(C): Carregar Jogo", 300, 300, FONT_SIZE, BLACK);
	DrawText("(Q): Sair do Jogo", 300, 350, FONT_SIZE, BLACK);
}