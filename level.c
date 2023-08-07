#include "level.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gamelib.h"
#include "level_lib.h"
#include "level_consts.h"
#include "debug.h"


#define millis2frames(millis) millis * FPS / 1000
#define matrix2screen(mpos) AddVec2(ScaleVec2(mpos, unit_length), level_offset)
#define screen2matrix(spos) DivideVector2(SubVector2(spos, Vector2FromVec2(level_offset)), unit_length)

typedef struct {
	Vec2 position;
	char is_any_enemy_at;
} is_enemy_at_Args;

void load_map(const char* file_name);
void set_unit_length(int length);
char is_in_bounds(Vec2 position);
char is_on_tile(Vec2 pos, Tile tile);
int spawn_enemy(Vec2 position);
void foreach_enemy(void (*callback)(PooledEnemy* enemy, void* context), void* context);
void update_enemy(PooledEnemy* pooled_enemy, void*);
void update_bullet();
void is_enemy_at(PooledEnemy* enemy, is_enemy_at_Args* args);
void player_enemy_collision();
void defrag_pool();
char is_in_sight(Vec2 pos);
void draw_tile(Vec2 position, Color color);
void draw_enemy(PooledEnemy* enemy, void*);


Vec2 level_size, level_offset;
int unit_length, sight_radius;

char /*Tile*/ map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];

// Object pooling é uma técnica de armazenar os objetos em uma array finita já desde o começo,
// reaproveitando os objetos já destruídos ou ainda não criados
typedef struct {
	// A pool de fato
	PooledEnemy pool[ENEMY_MAX];

	// Os bounds são para questões de performance.
	// Atualizando-os, não precisaremos avaliar todos os inimigos para verificar seus estados
	int lower_bound;
	int upper_bound;
} EnemyPool;

EnemyPool enemy_pool;
EnemyPool initial_enemy_pool;

Vec2 initial_player_position;
Vec2 player_position;

int bullet_lifetime;
Vector2 bullet_position;
Vector2 bullet_velocity;
int bullet_cooldown;
float bullet_speed;
Vec2 bullet_size;

char enemy_touches_player;


// Chamada quando o jogo deve inicializar
void Level_Init() {
	int i, j;

	SetTargetFPS(FPS);

	/* Inicializações */

	sight_radius = BASE_SIGHT_RADIUS;

	bullet_lifetime = 0;
	bullet_cooldown = 0;
	bullet_speed = BULLET_SPEED / FPS;

	enemy_touches_player = 0;

	for (i = 0; i < ENEMY_MAX; i++) {
		enemy_pool.pool[i].active = 0;
	}

	enemy_pool.lower_bound = 0;
	enemy_pool.upper_bound = 0;

	load_map("map.txt");
	
	// Spawnar entidades baseado no mapa
	for (i = 0; i < level_size.y; i++) {
		for (j = 0; j < level_size.x; j++) {
			Vec2 matrix_position = {j, i};

			switch (map[i][j]) {
				case T_PLAYER:
					player_position = initial_player_position = matrix_position;
					break;

				case T_ENEMY:
					spawn_enemy(matrix_position);
					break;
			}
		}
	}

	// Armazenar a pool de inimigos inicial para reset posterior
	memcpy(&initial_enemy_pool, &enemy_pool, sizeof(EnemyPool));
}

// Chamada em cada frame antes de Draw()
void Level_Update() {
	Vec2 input_dir = { 0, 0 }, target_position;

	/* Input de movimento */

	if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) input_dir.y--;
	if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) input_dir.y++;
	if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) input_dir.x--;
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) input_dir.x++;

	/* Tiro */

	bullet_cooldown--;

	if (IsMouseButtonPressed(0) || IsKeyPressed(KEY_G) || IsKeyPressed(KEY_SPACE)) {
		#if (!DEBUG_BULLET_NOCOOLDOWN)
		if (bullet_cooldown <= 0)
		#endif
		{
			/* Atirar */

			Vec2 mouse_pos = { GetMouseX(), GetMouseY() };
			Vec2 mouse_diff = SubVec2(mouse_pos, matrix2screen(player_position));

			bullet_position = Vector2FromVec2(AddVec2(matrix2screen(player_position), (Vec2){unit_length/2, unit_length/2}));
			bullet_velocity = ScaleVector2(NormalizeVector2(Vector2FromVec2(mouse_diff)), bullet_speed);
			bullet_cooldown = millis2frames(SHOT_COOLDOWN);
			bullet_lifetime = millis2frames(BULLET_LIFETIME);
		}
	}

	// Se a bala estiver ativa, atualizar seu estado
	if (bullet_lifetime > 0) {
		update_bullet();
	}

	/* Movimento do jogador */

	target_position = AddVec2(player_position, input_dir);

	// Desculpa pelo if esquisito, mas é a forma menos suja de enfiar a condicional de noclip
	if (
		is_in_bounds(target_position)
		
		#if (!DEBUG_NOCLIP)
		&& !is_on_tile(target_position, T_WALL)
		&& !is_on_tile(target_position, T_BURIED)
		#endif
	) {
		is_enemy_at_Args args;

		player_position = target_position;

		args.is_any_enemy_at = 0;
		args.position = player_position;

		// Testar colisão com inimigos
		foreach_enemy(&is_enemy_at, &args);

		if (args.is_any_enemy_at) {
			enemy_touches_player = 1;
		}
	}

	/* Movimento do inimigo */

	foreach_enemy(&update_enemy, NULL);

	/* Colisões */

	// Jogador e inimigo
	if (enemy_touches_player) {
		#if (!DEBUG_INVINCIBILITY)
		player_enemy_collision();
		#endif

		enemy_touches_player = 0;
	}
}

// Chamada entre BeginDrawing() e EndDrawing() em cada frame
void Level_Draw() {
	int i, j;

	// Limpar tela do frame anterior
	ClearBackground(BLACK);

	// Desenhar os tiles
	for (i = 0; i < level_size.y; i++) {
		Vec2 pos;

		pos.y = i;

		for (j = 0; j < level_size.x; j++) {
			pos.x = j;

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

				// Vazio
				default:
					draw_tile(pos, COLOR_EMPTY);
			}
		}
	}

	// Desenhar os inimigos
	foreach_enemy(&draw_enemy, NULL);

	// Desenhar o jogador
	draw_tile(player_position, COLOR_PLAYER);

	// Desenhar tiro (se estiver ativo)
	if (bullet_lifetime > 0) {
		BeginRotation(bullet_position, Vector2Angle(bullet_velocity) * RAD2DEG);
			DrawEllipse(0, 0, bullet_size.y, bullet_size.x, COLOR_BULLET);
		EndRotation();
	}
}

void load_map(const char* file_name) {
	FILE* file;

	// `fopen()` dava erro no VS pois é considerado deprecado.
	// Sei que parece gambiarra, mas `fopen_s()` é a opção mais segura
	// E também sei que parece errado o `&file`, sendo file já um `FILE*`
	// Mas sim, `fopen_s()` requer um FILE** como primeiro parâmetro
	// https://learn.microsoft.com/pt-br/cpp/c-runtime-library/reference/fopen-s-wfopen-s?view=msvc-170
	if (fopen_s(&file, file_name, "rb") != 0) {
		// Deu erro. Não abriu.
		printf("ERRO: Falha ao carregar mapa\n");
	} else {
		int i, j, line_start = 0;
		char map_buffer[MAX_LEVEL_WIDTH * MAX_LEVEL_HEIGHT] = {0};
		size_t bytes_read;
		Vec2 min_unit_length;

		// Deu certo. Abriu.

		// Ler todo o mapa que o arquivo continha (com um máximo)
		// 1 é somado a MAX_LEVEL_WIDTH porque o \n é um caractere que não é um elemento
		bytes_read = fread(&map_buffer, sizeof(char), (MAX_LEVEL_WIDTH + 1) * MAX_LEVEL_HEIGHT, file);
		fclose(file);

		// Valor temporário
		level_size.x = 0;

		// Interpretar cada linha (até \n)
		for (i = 0; line_start < bytes_read; i++) {
			for (j = 0; map_buffer[line_start + j] != '\n' && line_start + j <= bytes_read; j++) {
				map[i][j] = map_buffer[line_start + j];
			}

			// A largura do nível é setada várias vezes até a maior ser encontrada
			if (j > level_size.x) {
				level_size.x = j - 1;
			}

			line_start += j + 1;
		}

		level_size.y = i;

		/* Definir o comprimento unitário e o offset da tela */

		min_unit_length.x = GetScreenWidth()  / level_size.x; // Variável auxiliar. Mínimo para a largura ser satisfeita
		min_unit_length.y = GetScreenHeight() / level_size.y; // Mínimo para a altura ser satisfeita

		// O comprimento unitário é a menor largura de aresta de um tile que permite que o nível inteiro seja desenhado na tela
		if (min_unit_length.y > min_unit_length.x) {
			// O comprimento unitário se torna o menor entre os dois mínimos
			set_unit_length(min_unit_length.x);
		} else {
			set_unit_length(min_unit_length.y);
		}

		// O offset serve para centralizar o jogo na tela, deixando bordas pretas quando necessário
		level_offset.x = (GetScreenWidth()  - level_size.x * unit_length) / 2;
		level_offset.y = (GetScreenHeight() - level_size.y * unit_length) / 2;
	}
}

void set_unit_length(int length) {
	unit_length = length;
	bullet_size = (Vec2){ length * BULLET_DIAMETER/2, length * BULLET_LENGTH/2 };
}

// Verificar se a posição está dentro do nível
char is_in_bounds(Vec2 position) {
	return !(position.x < 0 || position.x >= level_size.x || position.y < 0 || position.y >= level_size.y);
}

// Verificar se a posição é de um tile de algum dos tipos especificados
char is_on_tile(Vec2 pos, Tile tile) {
	return map[pos.y][pos.x] == tile;
}

// Inicializar uma instância de inimigo em uma posição e atualizar a pool
int spawn_enemy(Vec2 position) {
	PooledEnemy* enemy;

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
	enemy->enemy.move_cooldown = millis2frames(INITIAL_MOVE_COOLDOWN);
	enemy->active = 1;
}

// Executar uma função para cada inimigo
// `callback` é a função a ser chamada, e seu parâmetro `context`, que será enviada diretamente pelo parâmetro `context` da função abaixo,
// é um ponteiro dinâmico (sem tipo) que serve como os argumentos adicionais (além do inimigo) que a função precise.
void foreach_enemy(void (*callback)(PooledEnemy* enemy, void* context), void* context) {
	int i;

	// Se houver `overflow` na pool, executa até o final da array e então começa do início até `upper_bound`
	if (enemy_pool.lower_bound <= enemy_pool.upper_bound) {
		for (i = enemy_pool.lower_bound; i <= enemy_pool.upper_bound; i++) {
			if (enemy_pool.pool[i].active) {
				(*callback)(&enemy_pool.pool[i], context);
			}
		}
	}
	else {
		for (i = enemy_pool.lower_bound; i <= ENEMY_MAX - 1; i++) {
			if (enemy_pool.pool[i].active) {
				(*callback)(&enemy_pool.pool[i], context);
			}
		}

		for (i = 0; i <= enemy_pool.upper_bound; i++) {
			if (enemy_pool.pool[i].active) {
				(*callback)(&enemy_pool.pool[i], context);
			}
		}
	}
}

// Atualizar o estado de um inimigo
// Compatível com `foreach_enemy()`
// O parâmetro `_` é ignorado
void update_enemy(PooledEnemy* pooled_enemy, void* _) {
	Enemy* enemy = &pooled_enemy->enemy;
	Vector2 enemy_on_screen;
	Rectangle enemy_hitbox;
	Rectangle bullet_hitbox;

	enemy->move_cooldown--;

	
	if (enemy->move_cooldown <= 0) {
		Vec2 target_position;

		// Decide aleatoriamente se a direção atual será alterada
		if (rand() < RAND_MAX * ENEMY_REDIRECT_CHANCE) {
			// Implementar: quanto mais próximo do jogador, mais bias ele tem de ir pro lado dele
		
			// Vec2 vector_distance = SubVec2(enemy->direction, player_position);
			// Vec2 bias = {1000, 1000};

			// bias.x /= vector_distance.x + 4;
			// bias.y /= vector_distance.y + 4;

			// do {
			//  	enemy->direction.x = GetRandomValue(-vector_distance.x * vector_distance.x, vector_distance.);
			//  	enemy->direction.y = GetRandomValue(-vector_distance.x, vector_distance.y);
			// } while (enemy->direction.x == 0 && enemy->direction.y == 0);
			
			do {
				enemy->direction.x = GetRandomValue(-1, 1);
				enemy->direction.y = GetRandomValue(-1, 1);
			} while (enemy->direction.x == 0 && enemy->direction.y == 0);
		}

		target_position = AddVec2(enemy->position, enemy->direction);

		// Note que se não for possível mover, o cooldown não é resetado
		if (is_in_bounds(target_position) && !is_on_tile(target_position, T_WALL)) {
			is_enemy_at_Args args;

			args.is_any_enemy_at = 0;
			args.position = target_position;

			// Colisão com os outros inimigos
			foreach_enemy(&is_enemy_at, &args);

			if (!args.is_any_enemy_at) {
				#if (!DEBUG_ENEMY_CANTMOVE)
				enemy->position = target_position;
				#endif			

				enemy->move_cooldown = GetRandomValue(millis2frames(MIN_ENEMY_MOVE_COOLDOWN), millis2frames(MAX_ENEMY_MOVE_COOLDOWN));

				// Se direção for uma diagonal, cooldown é multiplicado por raíz de dois
				if (enemy->direction.x * enemy->direction.x * enemy->direction.y * enemy->direction.y == 1) {
					enemy->move_cooldown = (int)(enemy->move_cooldown * SQRT_2);
				}

				// Testar se a nova posição encosta no jogador
				if (Vec2Equals(enemy->position, player_position)) {
					enemy_touches_player = 1;
				}
			}
		}
	}

	/* Colidir com tiro */
	enemy_on_screen = Vector2FromVec2(matrix2screen(enemy->position));
	enemy_hitbox = (Rectangle){ enemy_on_screen.x, enemy_on_screen.y, unit_length, unit_length };
	bullet_hitbox = (Rectangle){ bullet_position.x, bullet_position.y, bullet_size.x, bullet_size.x };

	if (bullet_lifetime > 0 && CheckCollisionRecs(enemy_hitbox, bullet_hitbox)) {
		pooled_enemy->active = 0;
		bullet_lifetime = 0;
	}
}

// Atualizar o estado da bala
void update_bullet() {
	Vec2 bullet_position_in_matrix;

	bullet_lifetime--;
	bullet_position = AddVector2(bullet_position, bullet_velocity);

	bullet_position_in_matrix = Vec2FromVector2(screen2matrix(bullet_position));

	if (is_in_bounds(bullet_position_in_matrix)) {
		char* tile = &map[bullet_position_in_matrix.y][bullet_position_in_matrix.x];

		switch (*tile) {
			#if (!DEBUG_BULLET_IGNOREWALLS)
			case T_WALL:
				bullet_lifetime = 0;
				break;
			#endif

			case T_BURIED:
				bullet_lifetime = 0;
				*tile = T_EMPTY;
				break;

			// Note que a colisão com o inimigo é tratada em `update_enemy()`
		}
	}
}

// O que acontece quando o jogador toca um inimigo
void player_enemy_collision() {
	// Resetar os inimigos
	memcpy(&enemy_pool, &initial_enemy_pool, sizeof(EnemyPool));

	// Resetar o jogador
	player_position = initial_player_position;
}

// Verificar se o inimigo está na posição
// Compatível com `foreach_enemy()`
void is_enemy_at(PooledEnemy* enemy, is_enemy_at_Args* args) {
	if (Vec2Equals(enemy->enemy.position, args->position)) {
		args->is_any_enemy_at = 1;
	}
}

// Desfragmentar a pool de inimigos
void defrag_pool() {
	int i;
	PooledEnemy defragged[ENEMY_MAX];

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

// Verificar se a posição é visível pelo jogador
char is_in_sight(Vec2 pos) {
	#if (DEBUG_NIGHTVISION)
	return 1;
	#endif

	int distance = Vec2Magnitude(SubVec2(pos, player_position));
	return distance < sight_radius;
}

// Desenhar um quadrado em uma posição da matriz
void draw_tile(Vec2 position, Color color) {
	Vec2 position_on_screen = matrix2screen(position);

	if (is_in_sight(position)) {
		DrawRectangle(position_on_screen.x, position_on_screen.y, unit_length, unit_length, color);
	}
}

// Desenhar um inimigo na tela
// Compatível com `foreach_enemy()`
// O parâmetro `_` é ignorado
void draw_enemy(PooledEnemy* enemy, void* _) {
	if (is_in_sight(enemy->enemy.position)) {
		draw_tile(enemy->enemy.position, COLOR_ENEMY);
	}
}
