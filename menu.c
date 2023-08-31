#include "level_lib.h"
#include <stdio.h>
#include "map_reader.c"
/*
#include "engine.h"
#include <stdlib.h>

#include <math.h>
#include <string.h>

//#include "level.c"
//

//#include "scene.h"

scene(Menu)


 //Não consegui dar um jeito de testar a posição dos inimigos...
int load_savestate(const char* path);
void back_to_level();
void new_game();

// Chamada quando o jogo deve inicializar
void Menu_Init() {

}

// Chamada entre BeginDrawing() e EndDrawing() em cada frame
void Menu_Draw() {
	
}

*/
char map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];
int score, lives, emeralds_collected;

int load_savestate(const char* path);
void read_enemy_position(PooledEnemy* enemy, FILE* fptr);

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
