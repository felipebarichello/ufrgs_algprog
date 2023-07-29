/*
* C�digo de mais alto n�vel relacionado ao jogo em si
*/


#include <string.h>
#include "raylib.h"
#include "gamelib.h"
#include "game.h"

#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 30

#define ENEMY_MAX 20
#define UNIT_LENGTH 20


struct {
	Vec2 direction;
} input;

Player player;

// Um pool maior que o necess�rio � mais otimizado que o uso da heap
Enemy enemy_pool[ENEMY_MAX] = {0};

// Inicializa o jogo
void Init() {
	player.position.x = 8;
	player.position.y = 8;
}

// Trata o input
void HandleInput() {
	input.direction.x = 0;
	input.direction.y = 0;

	if (IsKeyPressed(KEY_UP))    input.direction.y--;
	if (IsKeyPressed(KEY_DOWN))  input.direction.y++;
	if (IsKeyPressed(KEY_RIGHT)) input.direction.x++;
	if (IsKeyPressed(KEY_LEFT))  input.direction.x--;
}

// Chamado em cada frame antes de Draw()
void Update() {
	Vec2 new_position;

	new_position.x = player.position.x + input.direction.x;
	new_position.y = player.position.y + input.direction.y;

	// Movimento do jogador
	if (!is_solid(new_position)) {
		player.position = new_position;
	}
}

// Chamado entre BeginDrawing() e EndDrawing() em cada frame
void Draw() {
	ClearBackground(BLACK);

	// Desenhar o jogador
	DrawRectangle(player.position.x * UNIT_LENGTH, player.position.y * UNIT_LENGTH, UNIT_LENGTH, UNIT_LENGTH, GREEN);
}

// Verifica se a posi��o � s�lida e n�o pode ser atravessada
// Caracteriza-se como s�lido:
//   - Bordas do n�vel
//   - Paredes indestrut�veis
//   - �reas soterradas
int is_solid(Vec2 position) {
	int i;

	// Bordas do n�vel
	if (position.x < 0 || position.x >= LEVEL_WIDTH || position.y < 0 || position.y >= LEVEL_HEIGHT) {
		return 1;
	}

	// Paredes indestrut�veis
	for (i = 0; i < UNIT_LENGTH; i++) {
		// ...
	}
	
	// �reas soterradas
	// ...

	return 0;
}
