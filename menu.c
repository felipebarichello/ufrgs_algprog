#include "level_lib.h"
#include <stdio.h>
#include "map_reader.c"
#include "engine.h"
#include "level.h"
#include "scene.h"

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
	DrawText("Over Engineered Pac Mine", 250, 100, 36, RED);


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
		fscanf(fptr, "%d	%d	%d", &lives, &emeralds_collected, &score);
		}
		return 1;
}


void read_enemy_position(PooledEnemy* enemy, FILE* fptr) {
	fscanf(fptr, "%d	%d\n", enemy->enemy.position.x, enemy->enemy.position.y);
}


void new_game() {
	SetScene(Level_Scene());
}