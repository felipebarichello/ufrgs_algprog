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

// Object pooling � uma t�cnica de armazenar os objetos em uma array finita j� desde o come�o,
// reaproveitando os objetos j� destru�dos ou ainda n�o criados
struct {
	// O pool de fato
	struct PooledEnemy {
		Enemy enemy;
		int active; // Booleana para o estado do inimigo
	} pool[ENEMY_MAX];

	// Os bounds s�o para quest�es de performance.
	// Atualizando-os, n�o precisaremos avaliar todos os inimigos para verificar seus estados
	int lower_bound;
	int upper_bound;
} enemy_pool;

int is_solid(Vec2 position);
int spawn_enemy(Vec2 position);
void draw_in_matrix(Vec2 position, Color color);


// Inicializa o jogo
void Init() {
	int i;

	// Inicializar a posi��o do jogador
	player.position.x = 8;
	player.position.y = 8;

	// Inicializar os inimigos
	// Apenas � necess�rio inicializar os bounds e desativar os inimigos desnecess�rios
	for (i = 0; i < ENEMY_MAX; i++) {
		enemy_pool.pool[i].active = 0;
	}

	enemy_pool.lower_bound = 0;
	enemy_pool.upper_bound = 0;

	Vec2 pos;

	pos.x = 30;
	pos.y = 8;

	spawn_enemy(pos);
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
	int i;
	size_t min_bound, max_bound;

	// Limpar tela do frame anterior
	ClearBackground(BLACK);

	// Desenhar os inimigos
	if (enemy_pool.lower_bound < enemy_pool.upper_bound) {
		min_bound = enemy_pool.lower_bound;
		max_bound = enemy_pool.upper_bound;
	} else {
		min_bound = enemy_pool.upper_bound;
		max_bound = enemy_pool.lower_bound;
	}

	for (i = min_bound; i <= max_bound; i++) {
		if (enemy_pool.pool[i].active) {
			draw_in_matrix(enemy_pool.pool[i].enemy.position, RED);
		}
	}

	// Desenhar o jogador
	draw_in_matrix(player.position, GREEN);
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

int spawn_enemy(Vec2 position) {
	struct PooledEnemy* enemy;

	// Aqui, `upper_bound` pode se tornar igual a `lower_bound` ou sair para fora da array
	enemy_pool.upper_bound++;

	// Caso saia para fora da array
	if (enemy_pool.upper_bound == ENEMY_MAX) {
		enemy_pool.upper_bound = 0;
	}

	// Caso se torne igual a `lower_bound`, incrementar `lower_bound`
	if (enemy_pool.upper_bound == enemy_pool.lower_bound) {
		enemy_pool.lower_bound++;

		if (enemy_pool.lower_bound == ENEMY_MAX) {
			enemy_pool.lower_bound = 0;
		}
	}

	enemy = &enemy_pool.pool[enemy_pool.upper_bound];
	enemy->enemy.position = position;
	enemy->active = 1;
}

void draw_in_matrix(Vec2 position, Color color) {
	DrawRectangle(position.x * UNIT_LENGTH, position.y * UNIT_LENGTH, UNIT_LENGTH, UNIT_LENGTH, color);
}
