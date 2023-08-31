/*
* Inicialização da engine e tratamento pré-início e pós-término do jogo
*/

#include "start_engine.h"
#include "level.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


int main() {
	EngineBuilder builder;
	Level_Args level_args;

	builder.window_name = "Pac-Mine";
	builder.window_size = (Vec2){ WINDOW_WIDTH, WINDOW_HEIGHT };
	builder.initial_scene = Level_Scene();

	level_args.load_saved_game = 0;
	builder.initial_scene_args = &(Level_Args){0};

	StartEngine(&builder);

	return 0;
}
