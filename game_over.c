#include "level.h"
#include "level_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include "map_reader.h"
#include "engine.h"
#include "scene.h"
#include "level_consts.h"

scene(GameOver)

void GameOver_Init() {

}

void GameOver_Update() {
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

void GameOver_Draw() {
	ClearBackground(BLACK);
	DrawText("Game Over", 175, 100, 3 * FONT_SIZE, RED);
	DrawText("(N): Novo Jogo", 300, 250, FONT_SIZE, RAYWHITE);
	DrawText("(C): Carregar Jogo", 300, 300, FONT_SIZE, RAYWHITE);
	DrawText("(Q): Sair do Jogo", 300, 350, FONT_SIZE, RAYWHITE);
}