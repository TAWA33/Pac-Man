#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "algo.h"

#define RENDER_SCALE 0.7

typedef struct Affichage{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* map_texture;
    SDL_Texture* pacman_texture;
    SDL_Texture* ghost_textures[NB_GHOSTS][4]; // Haut - Bas - Gauche - Droite 
    SDL_Texture* tile_texture;
    SDL_Texture* highlight_texture;
    SDL_Texture* ghost_dizzy_texture;
    TTF_Font* font;
    SDL_Rect map_rect;
    SDL_Rect mouse_rect;
} Affichage;

int display_init(Affichage* d, EtatJeu* state);
void display_render(Affichage* d, EtatJeu* state);
void display_cleanup(Affichage* d);
void display_update_map(Affichage* d, EtatJeu* state);
void display_set_mouse_position(Affichage* d, int x, int y);

#endif