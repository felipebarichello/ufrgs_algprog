#include "map_reader.h"

void read_map(char map[][MAX_LEVEL_WIDTH], FILE* fptr) {
	for (int i = 0; i < MAX_LEVEL_HEIGHT; i++) {
		for (int j = 0; j < MAX_LEVEL_WIDTH; j++) {
			map[i][j] = fgetc(fptr);
		}
		fgetc(fptr);
	}
}
