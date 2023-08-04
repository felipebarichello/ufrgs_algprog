#include "level.h"

#include <stdlib.h>
#include <stdio.h>
#include "gamelib.h"
#include "level_lib.h"
#include "level_consts.h"

#define frames(millis) millis * FPS / 1000

void foreach_enemy(void (*callback)(Enemy* enemy, void* context), void* context);
char is_solid(Vec2 position);
int spawn_enemy(Vec2 position);
void update_enemy(Enemy* enemy, void*);
void defrag_pool();
void draw_tile(Vec2 position, Color color);
void draw_enemy(Enemy* enemy, void*);

Player player;

char /*Tile*/ map[LEVEL_WIDTH][LEVEL_HEIGHT] = {
	'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
	'#','J',' ',' ',' ',' ',' ','S','#',' ','T',' ',' ',' ','S','S','S','S','O','O','S','S',' ','#',' ','S','S','S','S','#',
	'#',' ',' ',' ','A',' ',' ',' ','#',' ',' ',' ',' ',' ','S','S','S','O','O','S','S','S',' ','#',' ','S','S','S','S','#',
	'#',' ','O',' ',' ',' ',' ',' ','#','#','#','#','#',' ',' ',' ','S','S','O','O','S','S',' ',' ',' ','S','E','O','O','#',
	'#','#','#','#','#','#','#',' ','S','E','S','S','S',' ',' ',' ','S','S','S','S','S','S',' ',' ',' ','S','S','S','S','#',
	'#','E','O','O','S','S','#','#','#','#','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ','S','S',' ',' ','#',
	'#','S','S','S','S','S','S','S',' ',' ',' ',' ',' ',' ',' ',' ','S','S','S','S','S','S',' ','#',' ',' ','A',' ',' ','#',
	'#',' ',' ','T',' ',' ',' ',' ',' ',' ',' ','A',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ','T','S','S',' ','#',
	'#','S','O','S','S','S','O','S',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','S','S','S',' ','#',
	'#','S','S','S','O','O','O','S','S','S',' ',' ',' ',' ','#','#','#','#','#','#','#',' ',' ',' ',' ','S','O','O',' ','#',
	'#','O','S','S','S','S','S','S','S','S','S',' ',' ',' ',' ',' ',' ',' ',' ',' ','T',' ',' ',' ',' ',' ',' ',' ',' ','#',
	'#','#','#','#','#','#','#','#','#','#','#','#','#','A',' ','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
	'#','T','S','S','S','S','S','S','S',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','S','S','S','E','#',
	'#','O','S','S','S','S','S','S',' ',' ',' ','#','S','S','S','S','S','#',' ',' ',' ','T',' ',' ',' ','S','O','O','S','#',
	'#','S','S','O','O','O','S','S',' ',' ',' ','#','S','S','O','O','S','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','S','S','#',
	'#',' ',' ','#','#','#','#','#','#','#','#','#','#','O','O','#','#','#','#','#','#','#','#','#','#','#','#','S','S','#',
	'#',' ',' ',' ',' ',' ','T',' ',' ','A',' ','S','S','S','S','S','S','S',' ',' ','T',' ',' ',' ','#',' ',' ',' ',' ','#',
	'#',' ','S','S','S','S','S','S','S',' ',' ',' ','S','S','S','S','S',' ',' ',' ',' ','#',' ',' ','#',' ','#',' ',' ','#',
	'#',' ','S','O','O','O','S','S','S',' ',' ',' ','S','S','E','S','S',' ',' ',' ',' ','#',' ',' ','A',' ','#',' ',' ','#',
	'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'
};

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

// Chamada quando o jogo deve inicializar
void Level_Init() {
	int i, j;

	SetTargetFPS(FPS);

	// Inicializar a pool de inimigos
	for (i = 0; i < ENEMY_MAX; i++) {
		enemy_pool.pool[i].active = 0;
	}

	enemy_pool.lower_bound = 0;
	enemy_pool.upper_bound = 0;

	// Ler o arquivo do mapa e transformá-lo nas entidades do mapa
	for (i = 0; i < LEVEL_WIDTH; i++) {
		for (j = 0; j < LEVEL_HEIGHT; j++) {
			Vec2 matrix_position;

			matrix_position.x = i;
			matrix_position.y = j;

			switch (map[i][j]) {
				case T_PLAYER:
					player.position = matrix_position;
					break;

				case T_ENEMY:
					spawn_enemy(matrix_position);
					break;
			}
		}
	}
}

// Chamada em cada frame antes de Draw()
void Level_Update() {
	Vec2 input_dir = {0, 0}, target_position;

	// Lidar com input
	if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) input_dir.y--;
	if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) input_dir.y++;
	if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) input_dir.x--;
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) input_dir.x++;

	// Movimento do jogador
	target_position = AddVec2(player.position, input_dir);

	if (!is_solid(target_position)) {
		player.position = target_position;
	}

	// Movimento do inimigo
	foreach_enemy(&update_enemy, NULL);
}

// Chamada entre BeginDrawing() e EndDrawing() em cada frame
void Level_Draw() {
	int i, j;

	// Limpar tela do frame anterior
	ClearBackground(BLACK);

	// Desenhar os tiles
	for (i = 0; i < LEVEL_WIDTH; i++) {
		for (j = 0; j < LEVEL_HEIGHT; j++) {
			Vec2 pos;

			pos.x = i;
			pos.y = j;

			switch (map[i][j]) {
				case T_WALL:
					draw_tile(pos, COLOR_WALL);
					break;

				case T_BURIED:
					draw_tile(pos, COLOR_BURIED);
					break;

				case T_EMERALD:
					draw_tile(pos, COLOR_EMERALD);
					break;

				case T_GOLD:
					draw_tile(pos, COLOR_GOLD);
					break;

				case T_POWERUP:
					draw_tile(pos, COLOR_POWERUP);
					break;
			}
		}
	}

	// Desenhar os inimigos
	foreach_enemy(&draw_enemy, NULL);

	// Desenhar o jogador
	draw_tile(player.position, COLOR_PLAYER);
}

// Verificar se a posição possui um tile sólido e não pode ser atravessada
// Para ver os tiles considerados sólidos, ver `level_lib.h`
char is_solid(Vec2 position) {
	int i, j;
	Tile tile;

	// Bordas do nível
	if (position.x < 0 || position.x >= LEVEL_WIDTH || position.y < 0 || position.y >= LEVEL_HEIGHT) {
		return 1;
	}

	// Paredes indestrutíveis e áreas soterradas
	switch (map[position.x][position.y]) {
		case T_WALL:
		case T_BURIED:
			return 1;
	}

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

	// Caso se torne igual a `lower_bound`, defragmentar pool ou sobrescrever antigo incrementar `lower_bound`
	if (enemy_pool.upper_bound == enemy_pool.lower_bound) {
		defrag_pool();

		if (enemy_pool.lower_bound == ENEMY_MAX - 1) {
			enemy_pool.lower_bound = 0;
		} else {
			enemy_pool.lower_bound++;
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

void defrag_pool() {
	int i;
	struct PooledEnemy defragged[ENEMY_MAX];

	for (i = 0; i < ENEMY_MAX - enemy_pool.lower_bound; i++) {
		defragged[i] = enemy_pool.pool[i + enemy_pool.lower_bound];
	}

	for (; i < ENEMY_MAX; i++) {
		defragged[i] = enemy_pool.pool[i - (ENEMY_MAX - enemy_pool.lower_bound)];
	}

	// Copiar de volta para a pool global
	for (i = 0; i < ENEMY_MAX; i++) {
		enemy_pool.pool[i] = defragged[i];
	}
}

// Desenhar um quadrado em uma posição da matriz
void draw_tile(Vec2 position, Color color) {
	DrawRectangle(position.x * UNIT_LENGTH, position.y * UNIT_LENGTH, UNIT_LENGTH, UNIT_LENGTH, color);
}

// Desenhar um inimigo na tela
// Compatível com `foreach_enemy()`
// O parâmetro `_` é ignorado
void draw_enemy(Enemy* enemy, void* _) {
	draw_tile(enemy->position, COLOR_ENEMY);
}
