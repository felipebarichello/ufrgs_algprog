/*
* Inicialização da engine
*/


#pragma once

#include "engine.h"


typedef struct EngineBuilder {
	const char* window_name;
	Vec2 window_size;
	Scene initial_scene;
	void* initial_scene_args;
} EngineBuilder;

void StartEngine(EngineBuilder*);
