#include "generate_lab.h"
#include "algo.h"
#include <stdio.h>
#include <time.h>
#include <string.h>


// Pour utiliser un algorithme de génération de labyrinthe on doit utiliser des stack
// https://en.wikipedia.org/wiki/Maze_generation_algorithm
// On utilise donc la manipulation des stak réalisé au DST (PG109)


struct stack_s {
    int capacity;
    int occupancy;
    int* content;
};

struct stack_s* stack_create(int capacity){
    if (capacity == 0){
        return NULL;
    }
    struct stack_s* s = malloc(sizeof(struct stack_s));
    if (s == NULL){
        return NULL;
    }
    s->capacity = capacity;
    s->occupancy = 0;
    s->content = malloc(s->capacity*sizeof(int));
    if (s->content == NULL){
        free(s);
        return NULL;
    }
    return s;
}

void stack_free(struct stack_s* stack){
    free(stack->content);
    free(stack);
}

int stack_push(struct stack_s* stack, int v){
    if (stack == NULL){
        return 0;
    }
    if (stack->occupancy == stack->capacity){
        return 0;
    }
    stack->content[stack->occupancy] = v;
    stack->occupancy++;
    return 1;
}

int stack_push_expand(struct stack_s* stack, int v){
    if (stack == NULL){
        return 0;
    }
    if (stack->occupancy == stack->capacity){
        int* new_content = realloc(stack->content, stack->capacity*2*sizeof(int));
        if (new_content == NULL){
            return 0;
        }
        stack->content = new_content;
        stack->capacity *= 2;
    }
    stack->content[stack->occupancy] = v;
    stack->occupancy++;
    return 1;
}

int stack_peek(struct stack_s* stack, int* v){
    if (stack == NULL || v == NULL || stack->occupancy == 0){
        return 0;
    }
    *v = stack->content[stack->occupancy-1];
    return 1;
}

int stack_pop(struct stack_s* stack, int* v){
    if (stack_peek(stack, v)){
        stack->occupancy--;
        return 1;
    }
    return 0;
}

int stack_occupancy(struct stack_s* stack){
    if (stack == NULL){
        return -1;
    }
    return stack->occupancy;
}

static void destroy_random_wall(enum tile_type_e** map, int nb_wall, int cells_w, int cells_h){
    int count = nb_wall;
    while (count>0){
        int x = rand() % (2 * cells_w + 1);
        int y = rand() % (2 * cells_h + 1);
        if (map[y][x] == WALL){
            map[y][x] = PATH;
            count--;
        }
    }
}


static enum tile_type_e** create_lab(int cells_w, int cells_h) {
    int tile_w = 2 * cells_w + 1;
    int tile_h = 2 * cells_h + 1;
 
    enum tile_type_e** map = malloc(tile_h * sizeof(enum tile_type_e*));
    if (!map){
        return NULL;
    }
    for (int y = 0; y < tile_h; y++) {
        map[y] = malloc(tile_w * sizeof(enum tile_type_e));
        if (!map[y]) {
            for (int k = 0; k < y; k++){
                free(map[k]);
            }
            free(map);
            return NULL;
        }
        memset(map[y], WALL, tile_w*sizeof(enum tile_type_e));
    }
    int total = cells_w * cells_h;
    int* visited = calloc(total, sizeof(enum tile_type_e));
    if (!visited) {
        for (int y = 0; y < tile_h; y++){
            free(map[y]);
        }
        free(map);
        return NULL;
    }
    struct stack_s* stack = stack_create(total + 1);
    if (!stack) { 
        free(visited); 
        return NULL; 
    }
    const int dx[4] = { 0,  0, -1,  1};
    const int dy[4] = {-1,  1,  0,  0};
 
    /* On suit l'algorithme décrit dans le lien.
     * 1. Choisir la cellule initiale, la marquer visitée, l'empiler.
     * 2. Tant que la pile n'est pas vide :
     *      a. Dépiler une cellule → cellule courante.
     *      b. Si elle a des voisines non visitées :
     *           - Rempiler la cellule courante (pour le backtracking).
     *           - Choisir aléatoirement une voisine non visitée.
     *           - Casser le mur entre la courante et la voisine.
     *           - Marquer la voisine visitée et l'empiler.
     * */
 
    visited[0] = 1;
    map[1][1]  = PATH;
    stack_push(stack, 0);
 
    while (stack_occupancy(stack) > 0) {
        int current;
        stack_pop(stack, &current);
 
        int cx = current % cells_w;
        int cy = current / cells_w;
 
        int unvisited[4];
        int count = 0;
        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d];
            int ny = cy + dy[d];
            if (nx >= 0 && nx < cells_w && ny >= 0 && ny < cells_h && !visited[ny * cells_w + nx]) {
                unvisited[count++] = d;
            }
        }
        if (count > 0) {
            stack_push(stack, current);

            int d = unvisited[rand() % count];
            int ncx = cx + dx[d];
            int ncy = cy + dy[d];

            map[2*cy + 1 + dy[d]][2*cx + 1 + dx[d]] = PATH;

            map[2*ncy + 1][2*ncx + 1] = PATH;

            visited[ncy * cells_w + ncx] = 1;
            stack_push(stack, ncy * cells_w + ncx);
        }
    }
    free(visited);
    stack_free(stack);
    return map;
}

static void place_pers(enum tile_type_e** map, int cells_w, int cells_h) {
    map[1][1] = PACMAN_START;
    int cx = cells_w / 2;
    int cy = cells_h / 2;

    int offsets[][2] = {
        {0, 0}, {1, 0}, {0, 1}, {-1, 0},
        {0,-1}, {1, 1}, {-1,1}, {1,-1}
    };
    int nb_offsets = (int)(sizeof(offsets) / sizeof(offsets[0]));
 
    int placed = 0;
    for (int i = 0; i < nb_offsets && placed < NB_GHOSTS; i++) {
        int gx = 2 * (cx + offsets[i][0]) + 1;
        int gy = 2 * (cy + offsets[i][1]) + 1;
 
        int tile_w = 2 * cells_w + 1;
        int tile_h = 2 * cells_h + 1;
 
        if (gx > 0 && gx < tile_w - 1 &&
            gy > 0 && gy < tile_h - 1 &&
            map[gy][gx] == PATH) {
            map[gy][gx] = GHOST_START;
            placed++;
        }
    }
}
 
int create_map_txt(const char* filename, int cells_w, int cells_h) {
    int tile_w = 2 * cells_w + 1;
    int tile_h = 2 * cells_h + 1;
 
    enum tile_type_e** map = create_lab(cells_w, cells_h);
    if (!map){
        return 0;
    }
    place_pers(map, cells_w, cells_h);
    destroy_random_wall(map, (tile_w * tile_h)/5, cells_w, cells_h);
 
    FILE* f = fopen(filename, "w");
    if (!f) {
        for (int y = 0; y < tile_h; y++) free(map[y]);
        free(map);
        return 0;
    }
    for (int y = 0; y < tile_h; y++) {
        for (int x = 0; x < tile_w; x++){
            switch(map[y][x]){
                case PATH:
                    fprintf(f, "%c", ' ');
                    break;
                case WALL:
                    fprintf(f, "%c", 'W');
                    break;
                case PACMAN_START:
                    fprintf(f, "%c", 'S');
                    break;
                case GHOST_START:
                    fprintf(f, "%c", 'G');
                    break;
                default:
                    fprintf(f, "%c", ' ');
                    break;
            }
        }
        fprintf(f, "%c", '\n');
    }
    fclose(f);
 
    for (int y = 0; y < tile_h; y++){
        free(map[y]);
    }
    free(map);
    return 1;
}