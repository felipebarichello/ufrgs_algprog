/*
* Define uma macro que será chamada por todos os arquivos que definem cenas.
*/


#pragma once

// Definir uma cena
// A macro declara três funções comuns a qualquer cena e define uma função que retorna uma `Scene`
#define scene(name)                                                     \
void name ## _Init(void* scene_args);                                   \
void name ## _Update(void* scene_data, void (*set_scene)(Scene scene)); \
void name ## _Draw(void* scene_data);                                   \
Scene name ## _Scene() {                                                \
	return (Scene) {                                                    \
		&name ## _Init,                                                 \
		&name ## _Update,                                               \
		&name ## _Draw                                                  \
	};                                                                  \
}
