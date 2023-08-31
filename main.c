/*
* Inicialização da engine e tratamento pré-início e pós-término do jogo
*/

#include "start_engine.h"
#include "level.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 800


int main() {
	EngineBuilder builder;

	builder.window_name = "Pac-Mine";
	builder.window_size = (Vec2){ WINDOW_WIDTH, WINDOW_HEIGHT };
	builder.initial_scene = Level_Scene();//Seria o caso de colocar a cena inicial como o menu principal e depois jogar pro level? 

	StartEngine(&builder);

	return 0;
}
