#include "algo.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>

enum tile_type_e default_map[N_Y_TILES][N_X_TILES] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 1, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
    }; // 0 = Mur ; 1 = Path ; 2 = Pacman Start; 3 = Ghost Start;

int get_dimensions(const char* filename, int* width, int* height){
    FILE* input_file = fopen(filename, "r");
    if (!input_file) {
        return EXIT_FAILURE;
    }
    int w = 0, h = 0;
    int max_w = 0;
    int c;
    
    while ((c = fgetc(input_file)) != EOF) {
        if (c == '\n') {
            if (w > max_w){
                max_w = w;
            }
            w = 0;
            h++;
        } else if (c == 'W' || c == ' ' || c == 'G' || c == 'S') {
            w++;
        }
    }

    if (w > 0) {
        if (w > max_w){
            max_w = w;
        }
        h++;
    }
    *width = max_w;
    *height = h;
    
    fclose(input_file);
    return EXIT_SUCCESS;
}

int load_map(const char* filename, EtatJeu* state){
    FILE* input_file = fopen(filename, "r");
    if (!input_file) {
        return 0;
    }
    if (get_dimensions(filename, &state->width, &state->height) != EXIT_SUCCESS) {
        fclose(input_file);
        return 0;
    }
    state->tiles = (enum tile_type_e**) malloc(state->height * sizeof(enum tile_type_e*));
    for (int i = 0; i < state->height; i++) {
        state->tiles[i] = (enum tile_type_e*) malloc(state->width * sizeof(enum tile_type_e));
        for (int j = 0; j < state->width; j++) {
            state->tiles[i][j] = PATH;
        }
    }
    fseek(input_file, 0, SEEK_SET);

    int c;
    int y = 0, x = 0;
    
    while ((c = fgetc(input_file)) != EOF && y < state->height) {
        if (c == '\n') {
            y++;
            x = 0;
        } else {
            if (y < state->height && x < state->width) {
                switch (c) {
                    case 'W': 
                        state->tiles[y][x] = WALL; 
                        break;
                    case ' ': 
                        state->tiles[y][x] = PATH; 
                        break;
                    case 'G': 
                        state->tiles[y][x] = GHOST_START; 
                        break;
                    case 'S': 
                        state->tiles[y][x] = PACMAN_START; 
                        break;
                    default: 
                        state->tiles[y][x] = WALL; 
                        break;
                }
            }
            x++;
        }
    }
    fclose(input_file);
    return 1; 
}

void apply_direction(int direction, int* x, int* y) {
    switch (direction) {
        case SDLK_UP:
            (*y)--;
            break;
        case SDLK_DOWN:
            (*y)++; 
            break;
        case SDLK_LEFT:
            (*x)--; 
            break;
        case SDLK_RIGHT:
            (*x)++; 
            break;
    }
}

int opposite_direction(int direction) {
    switch (direction) {
        case SDLK_UP:
            return SDLK_DOWN;
        case SDLK_DOWN:
            return SDLK_UP;
        case SDLK_LEFT:
            return SDLK_RIGHT;
        case SDLK_RIGHT:
            return SDLK_LEFT;
        default:
            return 0;
    }
}

int best_route(EtatJeu* state, int ghost_index, int* valid_dir, int nb_valid, int use_max) {
    if (nb_valid <= 0) return 0;
    
    int selected_dir = valid_dir[0];
    int extreme_dist = use_max ? 0 : 1000000;
    
    for (int i = 0; i < nb_valid; i++) {
        int nx = state->ghosts[ghost_index].x;
        int ny = state->ghosts[ghost_index].y;
        
        apply_direction(valid_dir[i], &nx, &ny);
        
        int dx = state->pacman_x - nx;
        int dy = state->pacman_y - ny;
        double dist = dx*dx + dy*dy;
        
        if (use_max) {
            if (dist > extreme_dist) {
                extreme_dist = dist;
                selected_dir = valid_dir[i];
            }
        } else {
            if (dist < extreme_dist) {
                extreme_dist = dist;
                selected_dir = valid_dir[i];
            }
        }
    }
    return selected_dir;
}

void game_init(EtatJeu* state, const char* map_file) {

    if (state->tiles != NULL) {
        game_cleanup(state);
    }
    state->tiles = NULL;
    state->width = 0;
    state->height = 0;

    if (map_file && load_map(map_file, state)) {
    } else {
        state->width = N_X_TILES;
        state->height = N_Y_TILES;
        
        state->tiles = (enum tile_type_e**)malloc(state->height * sizeof(enum tile_type_e*));
        for (int y = 0; y < state->height; y++) {
            state->tiles[y] = (enum tile_type_e*)malloc(state->width * sizeof(enum tile_type_e));
            for (int x = 0; x < state->width; x++) {
                state->tiles[y][x] = default_map[y][x];
            }
        }
    }
    int ghost_num = 0;

    for (int y = 0; y < state->height; y++) {
        for (int x = 0; x < state->width; x++) {
            if (state->tiles[y][x] == PACMAN_START) {
                state->pacman_x = x * TILE_SIZE;
                state->pacman_y = y * TILE_SIZE;
                state->tiles[y][x] = PATH;
            }
            if (state->tiles[y][x] == GHOST_START && ghost_num < NB_GHOSTS) {
                state->ghosts[ghost_num].x = x * TILE_SIZE;
                state->ghosts[ghost_num].y = y * TILE_SIZE;
                state->ghosts[ghost_num].direction = 0;
                state->ghosts[ghost_num].active = 1;
                ghost_num++;
                state->tiles[y][x] = PATH;
            }
        }
    }
    state->move_direction = 0;
    state->want_direction = 0;
    state->is_paused = 0;
    state->game_over = 0;
    state->nb_pacg = 0;
    state->old_nb_pacg = 0;
    state->vulnerable = 0;
    state->vulnerable_time = 0;
}

void teleport_test(EtatJeu* state, int* x, int* y) {
    int max_x = (state->width - 1) * TILE_SIZE;
    int max_y = (state->height - 1) * TILE_SIZE;

    if (*x < 0){
        *x = max_x;
    } else if (*x > max_x){
        *x = 0;
    } 
    if (*y < 0){
        *y = max_y;
    } else if (*y > max_y) {
        *y = 0;
    }
}

int is_collision(EtatJeu* state, int new_x, int new_y){
    int tile_x1 = new_x / TILE_SIZE; // POint en haut à gauche
    int tile_y1 = new_y / TILE_SIZE;

    int tile_x2 = (new_x+TILE_SIZE-1) / TILE_SIZE; // POint en haut à droite
    int tile_y2 = new_y / TILE_SIZE;

    int tile_x3 = new_x / TILE_SIZE; // POint en bas à gauche
    int tile_y3 = (new_y+TILE_SIZE-1) / TILE_SIZE;

    int tile_x4 = (new_x+TILE_SIZE-1) / TILE_SIZE; // POint en bas à droite
    int tile_y4 = (new_y+TILE_SIZE-1) / TILE_SIZE;

    if (tile_x1 < 0 || tile_x2 >= state->width || tile_y1 < 0 || tile_y4 >= state->height) { 
        return 1;
    }
    return (state->tiles[tile_y1][tile_x1] == WALL || state->tiles[tile_y2][tile_x2] == WALL ||
    state->tiles[tile_y3][tile_x3] == WALL || state->tiles[tile_y4][tile_x4] == WALL );
}


void game_update(EtatJeu* state) {
    if (state->is_paused || state->game_over) return;
    int pacman_moved = 0;

    if (state->want_direction != 0 && state->pacman_x%TILE_SIZE == 0 && state->pacman_y%TILE_SIZE == 0) {
        int new_x = state->pacman_x;
        int new_y = state->pacman_y;

        apply_direction(state->want_direction, &new_x, &new_y);
        teleport_test(state, &new_x, &new_y);
        
        if (!is_collision(state, new_x, new_y)) {
            state->pacman_x = new_x;
            state->pacman_y = new_y;
            state->move_direction = state->want_direction;
            pacman_moved = 1;
        }
    }
    if (!pacman_moved && state->move_direction != 0) {
        int new_x = state->pacman_x;
        int new_y = state->pacman_y;
              
        apply_direction(state->move_direction, &new_x, &new_y);
        teleport_test(state, &new_x, &new_y);  
        
        if (!is_collision(state, new_x, new_y)) {
            state->pacman_x = new_x;
            state->pacman_y = new_y;
        }
    }
    int tile_x = state->pacman_x / TILE_SIZE;
    int tile_y = state->pacman_y / TILE_SIZE;
    if (state->tiles[tile_y][tile_x] == PATH) {
        state->tiles[tile_y][tile_x] = PASSED;
        state->nb_pacg++;
    }
    if (state->nb_pacg != state->old_nb_pacg && state->nb_pacg % NB_GUM == 0 && state->nb_pacg != 0) {
        fprintf(stdout, "%d\n", state->nb_pacg);
        state->old_nb_pacg = state->nb_pacg;
        state->vulnerable = 1;
        state->vulnerable_time = SDL_GetTicks();
    }
    int dirs[4] = {SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT};

    for (int i = 0; i < NB_GHOSTS; i++) {
        if(state->ghosts[i].active){
            if (state->ghosts[i].x % TILE_SIZE == 0 && state->ghosts[i].y % TILE_SIZE == 0){
                int opposite = opposite_direction(state->ghosts[i].direction);

                int valid[4];
                int n_valid = 0;
                for (int d = 0; d < 4; d++) {
                    if (dirs[d] != opposite){

                    int nx = state->ghosts[i].x;
                    int ny = state->ghosts[i].y;
                    
                    apply_direction(dirs[d], &nx, &ny);
                    teleport_test(state, &nx, &ny);

                    if (!is_collision(state, nx, ny)) {
                        valid[n_valid++] = dirs[d];
                    }
                    }
                }
                if (n_valid == 0 && opposite != 0) {
                    valid[n_valid++] = opposite;
                }

                if (n_valid > 0) {
                    if (state->vulnerable){
                        state->ghosts[i].direction = best_route(state, i, valid, n_valid, 1);
                    } else {
                        state->ghosts[i].direction = best_route(state, i, valid, n_valid, 0);
                    }
                }
            }
            int nx = state->ghosts[i].x;
            int ny = state->ghosts[i].y;

            apply_direction(state->ghosts[i].direction, &nx, &ny);
            teleport_test(state, &nx, &ny);

            if (!is_collision(state, nx, ny)) {
                state->ghosts[i].x = nx;
                state->ghosts[i].y = ny;
            }
        }
    }

    if (state->vulnerable && SDL_GetTicks() - state->vulnerable_time > VULNERABLE_TIME) {
        state->vulnerable = 0;
    }
    SDL_Rect pacman_rect = {state->pacman_x, state->pacman_y, TILE_SIZE, TILE_SIZE};
    for (int i = 0; i < NB_GHOSTS; i++) {
        if (state->ghosts[i].active){
            SDL_Rect ghost_rect = {state->ghosts[i].x, state->ghosts[i].y, TILE_SIZE, TILE_SIZE};
            if (SDL_HasIntersection(&pacman_rect, &ghost_rect)) {
                if (state->vulnerable) {
                    state->ghosts[i].active = 0;
                } else {
                    state->game_over = 1;
                }
            }
        }
    }
}

void game_cleanup(EtatJeu* state) {
    if (state->tiles != NULL) {
        for (int i = 0; i < state->height; i++) {
            free(state->tiles[i]);
        }
        free(state->tiles);
        state->tiles = NULL;
        state->width = 0;
        state->height = 0;
    }
}