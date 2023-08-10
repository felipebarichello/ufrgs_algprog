/*
* Inicialização da engine
*/


#pragma once

#include "engine.h"


typedef struct {
	const char* window_name;
	Vec2 window_size;
	Scene initial_scene;
} EngineBuilder;


void StartEngine(EngineBuilder*);
