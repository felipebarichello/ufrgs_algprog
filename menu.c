#include "level_lib.h"
#include <stdio.h>
#include "map_reader.h"
#include "engine.h"
#include "level.h"
#include "scene.h"
#include "level_consts.h"

scene(Menu)

char map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];
int score, lives, emeralds_collected;

int load_savestate(const char* path);
void read_enemy_position(PooledEnemy* enemy, FILE* fptr);
void new_game();

void Menu_Init() {
	SetTargetFPS(FPS);
}

void Menu_Update() {
	
}

void Menu_Draw() {
	ClearBackground(RAYWHITE);

	DrawText("Over Engineered Pac Mine", 250, 100, 2*FONT_SIZE, RED);
	DrawText("(N): Novo Jogo", 250, 250, FONT_SIZE, BLACK);
	DrawText("(C): Carregar Jogo", 250, 300, FONT_SIZE, BLACK);
	DrawText("(Q): Sair do Jogo", 250, 350, FONT_SIZE, BLACK);
}

int load_savestate(const char* path) {
	FILE* fptr;
	FILE* buff;

	if (!(fptr = fopen_s(&buff, path, "r"))) {
		perror("Error in loading savestate");
		return 0;
	}
	else {
		read_map(map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH], fptr);
		fgetc(fptr);
		foreach_enemy(&read_enemy_position, &fptr);
		fgetc(fptr);
		fgetc(fptr);
		fscanf_s(fptr, "%d	%d	%d", &lives, &emeralds_collected, &score);
		}
		return 1;
}


void read_enemy_position(PooledEnemy* enemy, FILE* fptr) {
	fscanf_s(fptr, "%d	%d\n", enemy->enemy.position.x, enemy->enemy.position.y);
}


void new_game() {
	SetScene(Level_Scene());
}