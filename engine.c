#include "engine.h"
#include "start_engine.h"
#include <stdlib.h>
#include <time.h>


Scene cur_scene;
char scene_changed;

// Trocar a cena atual
void SetScene(Scene scene) {
	cur_scene = scene;
	scene_changed = 1;
}

void StartEngine(EngineBuilder* builder) {
	void* scene_data;

	srand(time(NULL));

	InitWindow(builder->window_size.x, builder->window_size.y, builder->window_name);
	InitAudioDevice();

	cur_scene = builder->initial_scene;
	scene_data = builder->initial_scene_args;
	scene_changed = 0;
	cur_scene.init(scene_data);

	while (!WindowShouldClose()) {
		cur_scene.update(scene_data, &SetScene);

		if (!scene_changed) {
			// Desenhar a cena atual se a cena for a mesma
			BeginDrawing();
			cur_scene.draw(scene_data);
			EndDrawing();
		} else {
			// Senão, inicializar a nova cena
			cur_scene.init(scene_data);
			scene_changed = 0;
		}
	}

	CloseAudioDevice();
	CloseWindow();
}
