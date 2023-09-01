#include "menu.h"

#include "level.h"
#include "level_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include "map_reader.h"
#include "engine.h"
#include "scene.h"
#include "level_consts.h"

scene(Menu)


int score, lives, emeralds_collected, next_level;
char load_level = 0;


void read_enemy_position(PooledEnemy* enemy, FILE* fptr);
void new_game();

void Menu_Init() {
	SetTargetFPS(FPS);
}

void Menu_Update(void (*set_scene)(Scene scene)) {

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

void Menu_Draw(void* _) {
	ClearBackground(RAYWHITE);
	DrawText("Over Engineered Pac Mine", 175, 100, 2 * FONT_SIZE, RED);
	DrawText("(N): Novo Jogo", 300, 250, FONT_SIZE, BLACK);
	DrawText("(C): Carregar Jogo", 300, 300, FONT_SIZE, BLACK);
	DrawText("(Q): Sair do Jogo", 300, 350, FONT_SIZE, BLACK);
}


/*


void read_enemy_position(PooledEnemy* enemy, FILE* fptr) {
	fscanf_s(fptr, "%d	%d\n", enemy->enemy.position.x, enemy->enemy.position.y);
}


void new_game() {
	SetScene(Level_Scene());
}
*/