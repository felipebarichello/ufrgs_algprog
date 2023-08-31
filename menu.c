#include "level_lib.h"
#include <stdio.h>
#include "map_reader.h"
#include "engine.h"
#include "level.h"
#include "scene.h"
#include "level_consts.h"
/*
scene(Menu)

char map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];
int score, lives, emeralds_collected, next_level;
char load_level = 0;


void read_enemy_position(PooledEnemy* enemy, FILE* fptr);
void new_game();

void Menu_Init() {
	SetTargetFPS(FPS);
}

void Menu_Update() {
	//As fun��es aqui n�o funcionam por enquanto - como � �bvio. S�o s� uma indica��o do que fazer quando conseguir passar as infos pro level
	if (IsKeyPressed(KEY_N)) {
		next_level = 1;
		SetScene(Level);
	}

	if (IsKeyPressed(KEY_C)) {
		SetScene(Level);
		load_savestate(savestate.txt);
	}

	if (IsKeyPressed(KEY_Q)) {
		CloseWindow(); //Essa aqui t� pronta
	}
}

void Menu_Draw() {

	InitWindow(800, 600, "Menu");
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Over Engineered Pac Mine", 175, 100, 2 * FONT_SIZE, RED);
		DrawText("(N): Novo Jogo", 300, 250, FONT_SIZE, BLACK);
		DrawText("(C): Carregar Jogo", 300, 300, FONT_SIZE, BLACK);
		DrawText("(Q): Sair do Jogo", 300, 350, FONT_SIZE, BLACK);
		EndDrawing();
	}
}




void read_enemy_position(PooledEnemy* enemy, FILE* fptr) {
	fscanf_s(fptr, "%d	%d\n", enemy->enemy.position.x, enemy->enemy.position.y);
}


void new_game() {
	SetScene(Level_Scene());
}
*/