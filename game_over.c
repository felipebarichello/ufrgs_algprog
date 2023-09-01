#include "game_over_scene.h"
#include "level_scene.h"
#include "level_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include "map_reader.h"
#include "engine.h"
#include "scene.h"
#include "level_consts.h"

scene(GameOver)

int score;
Ending ending;

void GameOver_Init(GameOver_Args* args) {
	ending = args->ending;
	score = args->score;

	free(args);
}

void GameOver_Update(void (*set_scene)(Scene scene)) {
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
		CloseWindow();
		return;
	}
}

void GameOver_Draw() {
	char print_score[32];

	
	switch (ending) {
		case VICTORY:
			ClearBackground(WHITE);
			DrawText("Vitória!", 300, 100, 3 * FONT_SIZE, GREEN);
			strcpy(print_score, "Score: ");
			sprintf(print_score, "%s%d", print_score, score);
			DrawText(print_score, 300, 200, FONT_SIZE, GRAY);
			DrawText("(N): Novo Jogo", 300, 250, FONT_SIZE, BLACK);
			DrawText("(C): Carregar Jogo", 300, 300, FONT_SIZE, BLACK);
			DrawText("(Q): Sair do Jogo", 300, 350, FONT_SIZE, BLACK);
		break;

		case DEFEAT:
			ClearBackground(BLACK);
			DrawText("Derrota!", 300, 100, 3 * FONT_SIZE, RED);
			strcpy(print_score, "Score: ");
			sprintf(print_score, "%s%d", print_score, score);
			DrawText(print_score, 300, 200, FONT_SIZE, BLUE);
			DrawText("(N): Novo Jogo", 300, 250, FONT_SIZE, RAYWHITE);
			DrawText("(C): Carregar Jogo", 300, 300, FONT_SIZE, RAYWHITE);
			DrawText("(Q): Sair do Jogo", 300, 350, FONT_SIZE, RAYWHITE);
		break;
	}
}