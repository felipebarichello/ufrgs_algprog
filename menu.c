#include "menu_scene.h"

#include "level_scene.h"
#include "level_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include "map_reader.h"
#include "engine.h"
#include "scene.h"
#include "level_consts.h"

scene(Menu)


void Menu_Init() {
	SetTargetFPS(FPS);
}

void Menu_Update(void (*set_scene)(Scene scene), void (*stop_engine)()) {
	if (IsKeyPressed(KEY_N)) {
		Level_Args* level_args = malloc(sizeof(Level_Args));
		level_args->load_saved_game = 0;
		set_scene(Level_Scene(), level_args);
		return;
	}

	if (IsKeyPressed(KEY_C)) {
		Level_Args* level_args = malloc(sizeof(Level_Args));
		level_args->load_saved_game = 1;
		set_scene(Level_Scene(), level_args);
		return;
	}

	if (IsKeyPressed(KEY_Q)) {
		(*stop_engine)();
		return;
	}
}

void Menu_Draw(void* _) {
	ClearBackground(RAYWHITE);
	DrawText("Overengineered", 175, 100, 3 * FONT_SIZE, RED);
	DrawText("Pac-Mine", 200, 140, 5 * FONT_SIZE, DARKBLUE);
	DrawText("(N): Novo Jogo", 270, 250, 1.2 * FONT_SIZE, BLACK);
	DrawText("(C): Carregar Jogo", 270, 280, 1.2 * FONT_SIZE, BLACK);
	DrawText("(Q): Sair do Jogo", 270, 310, 1.2 * FONT_SIZE, BLACK);
}
