/*
* Inicialização da engine e tratamento pré-início e pós-término do jogo
*/

#include "start_engine.h"
#include "menu.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


int main() {
	EngineBuilder builder;

	builder.window_name = "Pac-Mine";
	builder.window_size = (Vec2){ WINDOW_WIDTH, WINDOW_HEIGHT };
	builder.initial_scene = Menu_Scene();

	builder.initial_scene_args = NULL;

	StartEngine(&builder);

	return 0;
}
