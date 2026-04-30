#include <SDL2/SDL.h>
#ifndef ALGO_H
#define ALGO_H

#define N_X_TILES 28
#define N_Y_TILES 29
#define TILE_SIZE 40
#define NB_GHOSTS 4 
#define NB_GUM 30
#define VULNERABLE_TIME 2000

enum tile_type_e {
    WALL = 0,
    PATH,
    PACMAN_START,
    GHOST_START,
    PASSED,
};

typedef struct {
    int x, y;
    int direction;
    int active;
} Ghost;

typedef struct EtatJeu {
    enum tile_type_e** tiles;
    int width;
    int height;
    int pacman_x;
    int pacman_y;
    int move_direction;
    int want_direction;
    int is_paused;
    Ghost ghosts[NB_GHOSTS];
    int vulnerable;
    Uint32 vulnerable_time;
    int game_over;
    int nb_pacg;
    int old_nb_pacg;
} EtatJeu;

void game_init(EtatJeu* state, const char* map_file);
void game_update(EtatJeu* state);
void apply_direction(int direction, int* x, int* y);
void teleport_test(EtatJeu* state, int* x, int* y);
int opposite_direction(int direction);
void game_cleanup(EtatJeu* state);
int get_dimensions(const char* filename, int* width, int* height);
int load_map(const char* filename, EtatJeu* state);

#endif