#include "engine.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "scene.h"
#include "level_consts.h"
#include "level.c"

scene(menu)


int make_savestate(const char* path); //N�o consegui dar um jeito de testar a posi��o dos inimigos...
int load_savestate(const char* path);
void back_to_level();
void new_game();

int make_savestate(const char* path) { //Savestates s�o salvos como arquivos de texto, porque s� haver� suporte para um de cada vez 
	FILE* fptr;
	FILE* buff;
	is_enemy_at_Args args;
	int i, j;

	if (!(fptr = fopen_s(&buff, path, "w"))) {
		perror("Erro ao criar arquivo");
		return 0;
	}
	else {
		for (j = 0; j < MAX_LEVEL_WIDTH; j++) { //Escreve o mapa contido na tela quando o jogador chamou o menu, com inimigos e player
			for (i = 0; i < MAX_LEVEL_HEIGHT; i++) {//Acabei de me dar conta de um problema: as toupeiras podem acabar sobrescrevendo �reas soterradas. Seria melhor n�o mexer no mapa e s� guardar as posi��es de todas as toupeiras? Acho at� que � mais f�cil...
				args.position.x = j;
				args.position.y = i;
				if (player_position.x == j && player_position.y == i) {
					map[i][j] = T_PLAYER;
				}
				is_enemy_at(&enemy, &args);
				if (args.is_any_enemy_at) {
					map[i][j] == T_ENEMY;
				}

				fprintf(fptr, "%c", map[i][j]);
			}
			fprintf(fptr, "\n");
		}
		fprintf(fptr, "\n\n");
		fprintf(fptr, "%d	%d	%d", lives, level_completion, score);
		

		fclose(fptr);
		return 1;
	}
}

int load_savestate(const char* path) {
	FILE* fptr;
	FILE* buff;

	if (!(fptr = fopen_s(&buff, path, "r"))) {
		perror("Error in loading savestate");
		return 0;
	}
	else {
		
		return 1;
	}
}