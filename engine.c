#include "engine.h"
#include "start_engine.h"
#include <stdlib.h>
#include <time.h>


Scene cur_scene;

void StartEngine(EngineBuilder* builder) {
	srand(time(NULL));

	InitWindow(builder->window_size.x, builder->window_size.y, builder->window_name);
	InitAudioDevice();

	cur_scene = builder->initial_scene;

	cur_scene.init();

	while (!WindowShouldClose()) {
		cur_scene.update();

		BeginDrawing();
		cur_scene.draw();
		EndDrawing();
	}

	CloseAudioDevice();
	CloseWindow();
}
