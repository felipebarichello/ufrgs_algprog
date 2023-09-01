#include "level_scene.h"

#include "engine.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "level_lib.h"
#include "debug.h"

#include "scene.h"
#include "game_over_scene.h"
#include "victory_scene.h"

scene(Level)

typedef struct {
	Vec2 position;
	char is_any_enemy_at;
} is_enemy_at_Args;

void new_game();
void load_save();
void load_map(const char* file_name);
void load_sounds();
void update_level();
void update_pause(void (*set_scene)(Scene scene));
void soft_reset();
void set_unit_length(int length);
char is_in_bounds(Vec2 position);
char is_on_tile(Vec2 pos, Tile tile);
void spawn_enemy(Vec2 position, EnemyPool* pool);
void foreach_enemy(void (*callback)(PooledEnemy* enemy, void* context), void* context);
void update_enemy(PooledEnemy* pooled_enemy, void*);
void update_bullet();
void is_enemy_at(PooledEnemy* enemy, is_enemy_at_Args* args);
void player_enemy_collision();
void defrag_pool();
char is_in_sight(Vec2 pos);
void draw_tile(Vec2 position, Color color);
void draw_enemy(PooledEnemy* enemy, void*);
void check_collectable();
void print_emeralds(Vec2 caixa);
void print_score(Vec2 caixa);
void print_lives(Vec2 caixa);
int check_level_complete();
int make_savestate(const char* path);
void write_enemy_position(PooledEnemy* enemy, FILE* fptr);
int load_savestate(const char* path);


Vec2 level_size, level_offset;
int unit_length, sight_radius, emeralds_collected;
char map_name[18];

char /*Tile*/ map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];
int level_max_emeralds, max_level, next_level = 1;
char level_map_path[64];

EnemyPool enemy_pool;
EnemyPool initial_enemy_pool;

Vec2 initial_player_position;
Vec2 player_position;
int score, lives, powerup_timer;

int shot_cooldown;
float bullet_speed;
Vec2 bullet_size;

Bullet bullet;

char enemy_touches_player;

int combo;
int combo_timer;

LevelSounds sounds;

Vec2 text_box;

char paused;


// Chamada quando o jogo deve inicializar
void Level_Init(Level_Args* args) {
	text_box.x = 0;
	text_box.y = 0;

	SetTargetFPS(FPS);
	
	load_sounds();

	sight_radius = BASE_SIGHT_RADIUS;
	bullet_speed = BULLET_SPEED / FPS;

	emeralds_collected = 0;
	level_max_emeralds = 0;

	enemy_touches_player = 0;

	if (args->load_saved_game) {
		// load_save();
	} else {
		new_game();
	}

	soft_reset();

	paused = 0;

	free(args);
}

// Chamada em cada frame antes de Draw()
void Level_Update(void (*set_scene)(Scene scene)) {
	if (IsKeyPressed(KEY_TAB)) {
		paused = !paused;
	}

	if (!paused) {
		update_level();
	} else {
		update_pause(set_scene);
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
	if (bullet.lifetime > 0) {
		BeginRotation(bullet.position, Vector2Angle(bullet.velocity) * RAD2DEG);
			DrawEllipse(0, 0, bullet_size.y, bullet_size.x, COLOR_BULLET);
		EndRotation();
	}
	/*Imprimir informações de vidas, score e esmeraldas*/

	text_box.x = 0;
	print_lives(text_box);

	text_box.x += 260;
	print_emeralds(text_box);

	text_box.x += 260;
	print_score(text_box);


	// Se estiver pausado, escurecer o fundo e desenhar o menu de pausa
	if (paused) {
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
		DrawText("PAUSED", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 - 200, 100, WHITE);
		DrawText("(N): Novo Jogo",     250, 250, 24, WHITE);
		DrawText("(C): Carregar Jogo", 250, 300, 24, WHITE);
		DrawText("(S): Salvar Jogo",   250, 350, 24, WHITE);
		DrawText("(Q): Sair do Jogo",  250, 400, 24, WHITE);
		DrawText("(V): Voltar",        250, 450, 24, WHITE);
	}
}

void new_game() {
	int i, j;

	emeralds_collected = 0;
	level_max_emeralds = 0;
	lives = 3;
	score = 0;
	
	load_map("resources/maps/001.map");

	for (i = 0; i < ENEMY_MAX; i++) {
		initial_enemy_pool.pool[i].active = 0;
	}

	initial_enemy_pool.lower_bound = 0;
	initial_enemy_pool.upper_bound = 0;
	
	// Spawnar entidades e contar esmeraldas baseado no mapa
	for (i = 0; i < level_size.y; i++) {
		for (j = 0; j < level_size.x; j++) {
			Vec2 matrix_position = {j, i};

			switch (map[i][j]) {
				case T_PLAYER:
					player_position = initial_player_position = matrix_position;
					break;

				case T_ENEMY:
					spawn_enemy(matrix_position, &initial_enemy_pool);
					break;

				case T_EMERALD:
					level_max_emeralds++;
					break;
			}
		}
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
		perror("ERRO: Falha ao carregar mapa\n");
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

void load_sounds() {
	sounds.combo[0]  = LoadSound("resources/audio/firstblood.wav");
	sounds.combo[1]  = LoadSound("resources/audio/headshot.wav");
	sounds.combo[2]  = LoadSound("resources/audio/doublekill.wav");
	sounds.combo[3]  = LoadSound("resources/audio/triplekill.wav");
	sounds.combo[4]  = LoadSound("resources/audio/multikill.wav");
	sounds.combo[5]  = LoadSound("resources/audio/killingspree.wav");
	sounds.combo[6]  = LoadSound("resources/audio/rampage.wav");
	sounds.combo[7]  = LoadSound("resources/audio/dominating.wav");
	sounds.combo[8]  = LoadSound("resources/audio/ultrakill.wav");
	sounds.combo[9]  = LoadSound("resources/audio/godlike.wav");
	sounds.combo[10] = LoadSound("resources/audio/monsterkill.wav");
	sounds.gunshot   = LoadSound("resources/audio/gunshot.mp3");
	sounds.empty_gun = LoadSound("resources/audio/emptygun.wav");
	sounds.kill      = LoadSound("resources/audio/splat.mp3");
}

void update_level() {
	Vec2 input_dir = { 0, 0 }, target_position;
	
	/* Input de movimento */

	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
		input_dir.y--;
	if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
		input_dir.y++;
	if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
		input_dir.x--;
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
		input_dir.x++;

	/* Tiro */

	shot_cooldown--;
	combo_timer--;

	if (IsMouseButtonPressed(0) || IsKeyPressed(KEY_G) || IsKeyPressed(KEY_SPACE)) {
		#if (!DEBUG_BULLET_NOCOOLDOWN)
		if (shot_cooldown <= 0)
		#endif
		{
			/* Atirar */

			Vec2 mouse_pos = { GetMouseX(), GetMouseY() };
			Vec2 mouse_diff = SubVec2(mouse_pos, matrix2screen(player_position));

			bullet.position = Vector2FromVec2(AddVec2(matrix2screen(player_position), (Vec2) { unit_length / 2, unit_length / 2 }));
			bullet.velocity = ScaleVector2(NormalizeVector2(Vector2FromVec2(mouse_diff)), bullet_speed);
			shot_cooldown = millis2frames(SHOT_COOLDOWN);
			bullet.lifetime = millis2frames(BULLET_LIFETIME);

			PlaySound(sounds.gunshot);
		}
		#if (!DEBUG_BULLET_NOCOOLDOWN)
		else
		{
			PlaySound(sounds.empty_gun);
		}
		#endif
	}

	// Se a bala estiver ativa, atualizar seu estado
	if (bullet.lifetime > 0) {
		update_bullet();
	}

	/* Movimento do jogador */

	target_position = AddVec2(player_position, input_dir);

	// Desculpa pelo if esquisito, mas é a forma menos suja de enfiar a condicional de noclip
	if (
		is_in_bounds(target_position)

		#if (!DEBUG_PLAYER_NOCLIP)
			&& !is_on_tile(target_position, T_WALL) && !is_on_tile(target_position, T_BURIED)
		#endif
	) {
		is_enemy_at_Args args;

		player_position = target_position;

		args.is_any_enemy_at = 0;
		args.position = player_position;

		// Testar colisão com inimigos
		foreach_enemy(&is_enemy_at, &args);

		if (args.is_any_enemy_at)
		{
			enemy_touches_player = 1;
		}
	}

	/* Movimento do inimigo */

	foreach_enemy(&update_enemy, NULL);

	/* Colisões */

	//Jogador e coletáveis
	check_collectable();

	if (powerup_timer <= 0) {
		sight_radius = BASE_SIGHT_RADIUS;
	} else {
		powerup_timer--;
	}

	// Jogador e inimigo
	if (enemy_touches_player) {
		#if (!DEBUG_PLAYER_INVINCIBILITY)
			player_enemy_collision();
		#endif

		enemy_touches_player = 0;
	}

	/*Imprimir informações de vidas, score e esmeraldas*/

	text_box.x = 0;
	print_lives(text_box);

	text_box.x += 260;
	print_emeralds(text_box);

	text_box.x += 260;
	print_score(text_box);

	
	if (check_level_complete()) {
		next_level++;
		strcpy(level_map_path, "resources/maps/");
		strcpy(map_name, "mapa");
		sprintf(map_name, "%s%d", map_name, next_level);
		strcat(map_name, ".map");
		strcat(level_map_path, map_name);

		Level_Init(&((Level_Args){0}));
	}

	/*
	if (lives == 0) {
		set_scene(GameOver_Scene(), (int*)score);
	}
	else if (next_level > max_level) {
		set_scene(Victory_Scene(), (int*)score);
	}
	*/
}

void update_pause(void (*set_scene)(Scene scene)) {
	if (IsKeyPressed(KEY_N)) {
		Level_Args* level_args = malloc(sizeof(Level_Args));
		level_args->load_saved_game = 0;
		set_scene(Level_Scene(), level_args);
		return;
	}
	
	if (IsKeyPressed(KEY_C)) {
		Level_Args* level_args = malloc(sizeof(Level_Args));
		level_args->load_saved_game = 1;
		set_scene(Level_Scene(), level_args);
		return;
	}

	if (IsKeyPressed(KEY_S)) {
		return;
	}

	if (IsKeyPressed(KEY_Q)) {
		CloseWindow();
		return;
	}

	if (IsKeyPressed(KEY_V)) {
		paused = 0;
		return;
	}
}

// Reseta parcialmente o nível
void soft_reset() {
	// Resetar o jogador
	player_position = initial_player_position;
	powerup_timer = 0;

	// Resetar os inimigos
	memcpy(&enemy_pool, &initial_enemy_pool, sizeof(EnemyPool));

	// Resetar a bala
	bullet.lifetime = 0;
	shot_cooldown = 0;

	// Resetar o combo
	combo = -1;
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
void spawn_enemy(Vec2 position, EnemyPool* pool) {
	PooledEnemy* enemy;

	// Aqui, `upper_bound` pode se tornar igual a `lower_bound` ou sair para fora da array
	pool->upper_bound++;

	// Caso saia para fora da array
	if (pool->upper_bound == ENEMY_MAX) {
		pool->upper_bound = 0;
	}

	// Caso se torne igual a `lower_bound`, defragmentar pool ou sobrescrever antigo incrementar `lower_bound`
	if (pool->upper_bound == pool->lower_bound) {
		defrag_pool();

		if (pool->lower_bound == ENEMY_MAX - 1) {
			pool->lower_bound = 0;
		} else {
			pool->lower_bound++;
		}
	}

	enemy = &pool->pool[pool->upper_bound];
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
	bullet_hitbox = (Rectangle){ bullet.position.x, bullet.position.y, bullet_size.x, bullet_size.x };

	if (bullet.lifetime > 0 && CheckCollisionRecs(enemy_hitbox, bullet_hitbox)) {
		pooled_enemy->active = 0;
		bullet.lifetime = 0;

		score += ENEMY_POINTS;

		PlaySound(sounds.kill);

		if (combo == -1) {
			combo = 1;
			PlaySound(sounds.combo[0]);
		} else {
			if (combo_timer > 0) {
				combo++;
			} else {
				combo = 1;
			}

			if (combo < COMBO_SOUNDS) {
				PlaySound(sounds.combo[combo]);
			} else {
				PlaySound(sounds.combo[COMBO_SOUNDS - 1]);
			}
		}

		combo_timer = millis2frames(COMBO_DURATION);
	}
}

// Atualizar o estado da bala
void update_bullet() {
	Vec2 bullet_position_in_matrix;

	bullet.lifetime--;
	bullet.position = AddVector2(bullet.position, bullet.velocity);

	bullet_position_in_matrix = Vec2FromVector2(screen2matrix(bullet.position));

	if (is_in_bounds(bullet_position_in_matrix)) {
		char* tile = &map[bullet_position_in_matrix.y][bullet_position_in_matrix.x];

		switch (*tile) {
			#if (!DEBUG_BULLET_IGNOREWALLS)
			case T_WALL:
				bullet.lifetime = 0;
				break;
			#endif

			case T_BURIED:
				bullet.lifetime = 0;
				*tile = T_EMPTY;
				break;

			// Note que a colisão com o inimigo é tratada em `update_enemy()`
		}
	}
}

// O que acontece quando o jogador toca um inimigo
void player_enemy_collision() {
	soft_reset();
	lives--;
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

	#if (DEBUG_PLAYER_NIGHTVISION)
		return 1;
	#endif

	return Vec2Magnitude(SubVec2(pos, player_position)) < sight_radius // Verdadeiro se estiver dentro da visão do jogador
		|| ( // Ou se estiver dentro da visão da bala
			bullet.lifetime > 0 // Que exista
			&& Vec2Magnitude(SubVec2(pos, Vec2FromVector2(screen2matrix(bullet.position)))) < BULLET_SIGHT_RADIUS
		);
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

// Verificar se o jogador está em cima de um coletável
void check_collectable() {
	if (map[player_position.y][player_position.x] != T_EMPTY) {
		char *collectable = &map[player_position.y][player_position.x];

		switch (*collectable) {
			case T_EMERALD:
				score += EMERALD_POINTS;
				emeralds_collected++;
				*collectable = T_EMPTY;
				break;

			case T_GOLD:
				score += GOLD_POINTS;
				*collectable = T_EMPTY;
				break;

			case T_POWERUP:
				score += POWERUP_POINTS;
				sight_radius = 1000 * BASE_SIGHT_RADIUS;
				powerup_timer = POWERUP_DURATION * FPS;

				*collectable = T_EMPTY;
				break;
		}
	}
}

void print_lives(Vec2 caixa) { //Precisa ser chamada a cada contato com inimigo
	DrawText(TextFormat("Lives: %02i", lives), caixa.x, caixa.y, FONT_SIZE, RED);
}

void print_score(Vec2 caixa) { //Precisa ser chamada cada vez que um item for coletado
	char score_string[24];

	strcpy(score_string, "Score: ");
	sprintf(score_string, "%s%d", score_string, score);
	DrawText(score_string, caixa.x, caixa.y, FONT_SIZE, COLOR_SCORE);
}

void print_emeralds(Vec2 caixa) { //Precisa ser chamada toda vez que uma esmeralda for coletada
	char emerald_string[18];

	strcpy(emerald_string, "Emeralds: ");
	sprintf(emerald_string, "%s%d", emerald_string, emeralds_collected);
	strcat(emerald_string, "/");
	sprintf(emerald_string, "%s%d", emerald_string, level_max_emeralds);
	DrawText(emerald_string, caixa.x, caixa.y, FONT_SIZE, COLOR_EMERALD_TEXT);
}

int check_level_complete() {
	return emeralds_collected >= level_max_emeralds;
}

int make_savestate(const char* path) { //Savestates são salvos como arquivos de texto, porque só haverá suporte para um de cada vez 
	FILE* fptr;
	FILE* buff;
	//is_enemy_at_Args args;
	int i, j;
	const int const_neg_um = -1;

	if (!(fptr = fopen_s(&buff, path, "w"))) {
		perror("Erro ao criar arquivo");
		return 0;
	} else {
		for (j = 0; j < MAX_LEVEL_WIDTH; j++) { //Escreve o mapa contido na tela quando o jogador chamou o menu, com inimigos e player
			for (i = 0; i < MAX_LEVEL_HEIGHT; i++) {

				if (player_position.x == j && player_position.y == i) {
					map[i][j] = T_PLAYER;
				}

				fprintf(fptr, "%c", map[i][j]);
			}

			fprintf(fptr, "\n");
		}

		foreach_enemy(&write_enemy_position, &fptr);
		fprintf(fptr, "%d %d", const_neg_um, const_neg_um);
		fprintf(fptr, "\n\n");
		fprintf(fptr, "%d	%d	%d", lives, emeralds_collected, score);


		fclose(fptr);
		return 1;
	}
}


//Compatível com foreach_enemy
void write_enemy_position(PooledEnemy* enemy, FILE* fptr) {
	fprintf(fptr, "%d	%d\n", enemy->enemy.position.x, enemy->enemy.position.y);
}

int load_savestate(const char* path) {
	FILE* fptr;
	FILE* buff;
	Vec2 aux_vec2;
	EnemyPool pool;

	if (!(fptr = fopen_s(&buff, path, "r"))) {
		perror("Error in loading savestate");
		return 0;
	} else {
		read_map(map[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH], fptr);
		fgetc(fptr);
		do {
			fscanf_s(fptr, "%d", &aux_vec2.x);
			_fgetchar(fptr);
			fscanf_s(fptr, "%d", &aux_vec2.y);
			_fgetchar(fptr);
			spawn_enemy(aux_vec2, &pool);
		} while (aux_vec2.y != -1);
		fgetc(fptr);
		fgetc(fptr);
		fscanf_s(fptr, "%d	%d	%d", &lives, &emeralds_collected, &score);
	}

	return 1;
}