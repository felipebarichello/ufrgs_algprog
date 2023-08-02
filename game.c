/*
* Código de mais alto nível relacionado ao jogo em si
*/


#include <stdlib.h>
#include "raylib.h"
#include "gamelib.h"
#include "game.h"

#define FPS 60

#define ENEMY_MAX 20

#define UNIT_LENGTH 20
#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 30

#define MIN_ENEMY_MOVE_COOLDOWN 100
#define MAX_ENEMY_MOVE_COOLDOWN 150
#define ENEMY_REDIRECT_CHANCE   .2
#define INITIAL_MOVE_COOLDOWN   1200

#define frames(millis) millis * FPS / 1000

struct {
	Vec2 direction;
} input;

Player player;

// Object pooling é uma técnica de armazenar os objetos em uma array finita ja desde o começo,
// reaproveitando os objetos já destruídos ou ainda não criados
struct {
	// O pool de fato
	struct PooledEnemy {
		Enemy enemy;
		int active; // Booleana para o estado do inimigo
	} pool[ENEMY_MAX];

	// Os bounds são para questões de performance.
	// Atualizando-os, não precisaremos avaliar todos os inimigos para verificar seus estados
	int lower_bound;
	int upper_bound;
} enemy_pool;

void foreach_enemy(void (*callback)(Enemy* enemy, void* context), void* context);
int is_solid(Vec2 position);
int spawn_enemy(Vec2 position);
void update_enemy(Enemy* enemy, void*);
void draw_in_matrix(Vec2 position, Color color);
void draw_enemy(Enemy* enemy, void*);


// Chamada quando o jogo deve inicializar
void Init() {
	int i;

	SetTargetFPS(FPS);

	// Inicializar a posição do jogador
	player.position.x = 8;
	player.position.y = 8;

	// Inicializar os inimigos
	// Apenas é necessário inicializar os bounds e desativar os inimigos desnecess�rios
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

// Chamada quando se deve tratar o input
void HandleInput() {
	input.direction.x = 0;
	input.direction.y = 0;

	if (IsKeyPressed(KEY_UP))    input.direction.y--;
	if (IsKeyPressed(KEY_DOWN))  input.direction.y++;
	if (IsKeyPressed(KEY_RIGHT)) input.direction.x++;
	if (IsKeyPressed(KEY_LEFT))  input.direction.x--;
}

// Chamada em cada frame antes de Draw()
void Update() {
	Vec2 target_position;

	// Movimento do jogador
	target_position = AddVec2(player.position, input.direction);

	if (!is_solid(target_position)) {
		player.position = target_position;
	}

	// Movimento do inimigo
	foreach_enemy(&update_enemy, NULL);
}

// Chamada entre BeginDrawing() e EndDrawing() em cada frame
void Draw() {
	// Limpar tela do frame anterior
	ClearBackground(BLACK);

	// Desenhar os inimigos
	foreach_enemy(&draw_enemy, NULL);

	// Desenhar o jogador
	draw_in_matrix(player.position, GREEN);
}

// Verificar se a posição é sólida e não pode ser atravessada
// Caracteriza-se como s�lido:
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

// Mover um objeto sólido caso não hajam outros objetos sólidos no local
void try_move(Vec2* current, Vec2 target) {
	if (!is_solid(target)) {
		*current = target;
	}
}

// Inicializar uma instância de inimigo em uma posição e atualizar a pool
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
	enemy->enemy.direction = (Vec2){0, 0};
	enemy->enemy.move_cooldown = frames(INITIAL_MOVE_COOLDOWN);
	enemy->active = 1;
}

// Executar uma função para cada inimigo
// `callback` é a função a ser chamada, e seu parâmetro `context`, que será enviada diretamente pelo parâmetro `context` da função abaixo,
// é um ponteiro dinâmico (sem tipo) que serve como os argumentos adicionais (além do inimigo) que a função precise.
void foreach_enemy(void (*callback)(Enemy* enemy, void* context), void* context) {
	int i;

	// Se houver `overflow` na pool, executa até o final da array e então começa do início até `upper_bound`
	if (enemy_pool.lower_bound <= enemy_pool.upper_bound) {
		for (i = enemy_pool.lower_bound; i <= enemy_pool.upper_bound; i++) {
			if (enemy_pool.pool[i].active) {
				(*callback)(&enemy_pool.pool[i].enemy, context);
			}
		}
	}
	else {
		for (i = enemy_pool.lower_bound; i <= ENEMY_MAX - 1; i++) {
			if (enemy_pool.pool[i].active) {
				(*callback)(&enemy_pool.pool[i].enemy, context);
			}
		}

		for (i = 0; i <= enemy_pool.upper_bound; i++) {
			if (enemy_pool.pool[i].active) {
				(*callback)(&enemy_pool.pool[i].enemy, context);
			}
		}
	}
}

// Mover o inimigo
// Compatível com `foreach_enemy()`
// O parâmetro `_` é ignorado
void update_enemy(Enemy* enemy, void* _) {
	enemy->move_cooldown--;

	if (enemy->move_cooldown <= 0) {
		Vec2 target_position;

		// Decide aleatoriamente se a direção atual será alterada
		if (rand() < RAND_MAX * ENEMY_REDIRECT_CHANCE) {
			// Implementar: quanto mais próximo do jogador, mais bias ele tem de ir pro lado dele
		
			//Vec2 vector_distance = SubVec2(enemy->direction, player.position);
			//Vec2 bias = {1000, 1000};

			//bias.x /= vector_distance.x + 4;
			//bias.y /= vector_distance.y + 4;

			//do {
			//	enemy->direction.x = GetRandomValue(-vector_distance.x * vector_distance.x, vector_distance.);
			//	enemy->direction.y = GetRandomValue(-vector_distance.x, vector_distance.y);
			//} while (enemy->direction.x == 0 && enemy->direction.y == 0);
			
			do {
				enemy->direction.x = GetRandomValue(-1, 1);
				enemy->direction.y = GetRandomValue(-1, 1);
			} while (enemy->direction.x == 0 && enemy->direction.y == 0);
		}

		target_position = AddVec2(enemy->position, enemy->direction);

		// Note que se não for possível mover, o cooldown não é resetado
		if (!is_solid(target_position)) {
			enemy->position = target_position;

			enemy->move_cooldown = GetRandomValue(frames(MIN_ENEMY_MOVE_COOLDOWN), frames(MAX_ENEMY_MOVE_COOLDOWN));

			// Se direção for uma diagonal, cooldown é multiplicado por raíz de dois
			if (enemy->direction.x * enemy->direction.x * enemy->direction.y * enemy->direction.y == 1) {
				enemy->move_cooldown *= SQRT_2;
			}
		}
	}
}

// Desenhar um quadrado em uma posição da matrix
void draw_in_matrix(Vec2 position, Color color) {
	DrawRectangle(position.x * UNIT_LENGTH, position.y * UNIT_LENGTH, UNIT_LENGTH, UNIT_LENGTH, color);
}

// Desenhar um inimigo na tela
// Compatível com `foreach_enemy()`
// O parâmetro `_` é ignorado
void draw_enemy(Enemy* enemy, void* _) {
	draw_in_matrix(enemy->position, RED);
}
