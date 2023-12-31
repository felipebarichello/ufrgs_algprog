#include "gamelib.h"
#include "debug.h"


/* --- Computador --- */

#define FPS 60


/* --- Recursos --- */

#define COMBO_SOUNDS 11 // Total de sons de combo


/* --- Visual --- */

// Tiles
#define COLOR_EMPTY   GRAY
#define COLOR_WALL    (Color){ 60, 60, 60, 255 }
#define COLOR_BURIED  DARKBROWN
#define COLOR_EMERALD GREEN
#define COLOR_GOLD    GOLD
#define COLOR_POWERUP PURPLE

// Entidades
#define COLOR_PLAYER  BLUE
#define COLOR_ENEMY   RED
#define COLOR_BULLET  PINK


/* --- Nível --- */

#define MAX_LEVEL_WIDTH  100
#define MAX_LEVEL_HEIGHT 100


/* --- Jogo --- */

// Jogador
#define BASE_SIGHT_RADIUS   5    // Raio de visão base, sem powerups
#define BULLET_LIFETIME     1000 // Tempo de vida de um tiro
#define SHOT_COOLDOWN       2000 // Delay entre tiros
#define BULLET_SPEED        600  // Velocidade da bala
#define BULLET_LENGTH       1.f  // Comprimento visual e da hitbox da bala
#define BULLET_DIAMETER     .3f  // Diâmetro visual e da hitbox da bala
#define BULLET_SIGHT_RADIUS 4    // Raio de visão da bala
#define COMBO_DURATION      8000 // Tempo até o combo resetar

// Inimigo
#define ENEMY_MAX 200                // Máximo de inimigos na pool
#define MIN_ENEMY_MOVE_COOLDOWN 100  // Delay mínimo para o movimento do inimigo
#define MAX_ENEMY_MOVE_COOLDOWN 150  // Delay máximo para o movimento do inimigo
#define INITIAL_MOVE_COOLDOWN   1200 // Delay fixo inicial para o início do movimento do inimigo
#define ENEMY_REDIRECT_CHANCE   .2   // Chance de o inimigo mudar de direção a cada passo
#define ENEMY_POINTS 200

//Coletáveis
#define EMERALD_POINTS 100 //Incremento na pontuação ao coletar esmeraldas
#define GOLD_POINTS 50 //Incremento na pontuação ao coletar ouro
#define POWERUP_POINTS 200 //Incremento na pontuação ao coletar powerups

//Power-Up's
#define POWERUP_DURATION 3 //Duração do Powerup; 3s vezes o número de frames por segundo

//Texto em tela
#define COLOR_LIVES_TEXT   RED
#define COLOR_EMERALD_TEXT GREEN
#define COLOR_LEVEL_TEXT   YELLOW
#define COLOR_SCORE_TEXT   BLUE
#define FONT_SIZE 18
