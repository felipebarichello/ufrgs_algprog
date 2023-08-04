#include "gamelib.h"


/* --- Computador --- */

#define FPS 60


/* --- Visual --- */

#define COLOR_PLAYER  BLUE
#define COLOR_ENEMY   RED
#define COLOR_WALL    DARKGRAY
#define COLOR_BURIED  GRAY
#define COLOR_EMERALD GREEN
#define COLOR_GOLD    GOLD
#define COLOR_POWERUP PURPLE


/* --- Tamanhos --- */

#define MAX_LEVEL_WIDTH  100
#define MAX_LEVEL_HEIGHT 100


/* --- Jogo --- */

// Inimigo
#define ENEMY_MAX 20                 // Máximo de inimigos na pool
#define MIN_ENEMY_MOVE_COOLDOWN 100  // Delay mínimo para o movimento do inimigo
#define MAX_ENEMY_MOVE_COOLDOWN 150  // Delay máximo para o movimento do inimigo
#define INITIAL_MOVE_COOLDOWN   1200 // Delay fixo inicial para o início do movimento do inimigo
#define ENEMY_REDIRECT_CHANCE   .2   // Chance de o inimigo mudar de direção a cada passo
