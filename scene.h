/*
* Define uma macro que será chamada por todos os arquivos que definem cenas.
*/


#pragma once

// Definir uma cena
// A macro declara três funções comuns a qualquer cena e define uma função que retorna uma `Scene`
#define scene(name)        \
void name ## _Init();      \
void name ## _Update();    \
void name ## _Draw();      \
Scene name ## _Scene() {   \
	return (Scene) {       \
		&name ## _Init,    \
		&name ## _Update,  \
		&name ## _Draw     \
	};                     \
}
