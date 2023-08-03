/* --- Computador --- */

#define FPS 60


/* --- Visual --- */

#define COLOR_PLAYER GREEN
#define COLOR_ENEMY  RED
#define COLOR_WALL   GRAY
#define COLOR_BURIED LIGHTGRAY


/* --- Tamanhos --- */

#define UNIT_LENGTH 20

#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 30


/* --- Jogo --- */

// Inimigo
#define ENEMY_MAX 20                 // Máximo de inimigos na pool
#define MIN_ENEMY_MOVE_COOLDOWN 100  // Delay mínimo para o movimento do inimigo
#define MAX_ENEMY_MOVE_COOLDOWN 150  // Delay máximo para o movimento do inimigo
#define INITIAL_MOVE_COOLDOWN   1200 // Delay fixo inicial para o início do movimento do inimigo
#define ENEMY_REDIRECT_CHANCE   .2   // Chance de o inimigo mudar de direção a cada passo
