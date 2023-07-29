/*
* Código de mais alto nível relacionado ao jogo em si
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

// Um pool maior que o necessário é mais otimizado que o uso da heap
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

// Verifica se a posição é sólida e não pode ser atravessada
// Caracteriza-se como sólido:
//   - Bordas do nível
//   - Paredes indestrutíveis
//   - Áreas soterradas
int is_solid(Vec2 position) {
	int i;

	// Bordas do nível
	if (position.x < 0 || position.x >= LEVEL_WIDTH || position.y < 0 || position.y >= LEVEL_HEIGHT) {
		return 1;
	}

	// Paredes indestrutíveis
	for (i = 0; i < UNIT_LENGTH; i++) {
		// ...
	}
	
	// Áreas soterradas
	// ...

	return 0;
}
