#include "affichage.h"
#include <stdio.h>

static void set_window_position(SDL_Window* window) {
    int mouse_x, mouse_y;
    SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
    
    int nb_displays = SDL_GetNumVideoDisplays();
    SDL_Rect display_bounds;
    for (int i = 0; i < nb_displays; i++) {
        SDL_GetDisplayBounds(i, &display_bounds);
        if (mouse_x >= display_bounds.x && mouse_x < display_bounds.x + display_bounds.w &&
            mouse_y >= display_bounds.y && mouse_y < display_bounds.y + display_bounds.h) {
            break;
        }
    }
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_SetWindowPosition(window,
        display_bounds.x + (display_bounds.w - w) / 2,
        display_bounds.y + (display_bounds.h - h) / 2);
}

static int direction_to_index(int direction) {
    switch (direction) {
        case SDLK_UP:    
            return 0;
        case SDLK_DOWN:  
            return 1;
        case SDLK_LEFT:  
            return 2;
        case SDLK_RIGHT: 
            return 3;
        default:
            return 2;
    }
}

int display_init(Affichage* aff, EtatJeu* state) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 0;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        return 0;
    }
    int window_w = state->width * TILE_SIZE * RENDER_SCALE;
    int window_h = state->height * TILE_SIZE * RENDER_SCALE;
    
    aff->window = SDL_CreateWindow("Pacman",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_w, window_h, 0);
    if (!aff->window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 0;
    }
    set_window_position(aff->window);
    
    aff->renderer = SDL_CreateRenderer(aff->window, -1, SDL_RENDERER_SOFTWARE);
    if (!aff->renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_RenderSetScale(aff->renderer, RENDER_SCALE, RENDER_SCALE);
    aff->pacman_texture = IMG_LoadTexture(aff->renderer, "images/pacman-full.png");
    aff->ghost_dizzy_texture = IMG_LoadTexture(aff->renderer, "images/ghost-blue-dizzy.png");
    
    const char* colors[NB_GHOSTS] = {"red", "pink", "blue", "orange"};

    for (int i = 0; i < NB_GHOSTS; i++) {
        char link[64];
        sprintf(link, "images/ghost-%s-up.png",    colors[i]); 
        aff->ghost_textures[i][0] = IMG_LoadTexture(aff->renderer, link);

        sprintf(link, "images/ghost-%s-down.png",  colors[i]); 
        aff->ghost_textures[i][1] = IMG_LoadTexture(aff->renderer, link);

        sprintf(link, "images/ghost-%s-left.png",  colors[i]); 
        aff->ghost_textures[i][2] = IMG_LoadTexture(aff->renderer, link);

        sprintf(link, "images/ghost-%s-right.png", colors[i]); 
        aff->ghost_textures[i][3] = IMG_LoadTexture(aff->renderer, link);
    }
    aff->font = TTF_OpenFont("opensansregular.ttf", 24);
    
    if (!aff->pacman_texture || !aff->font) {
        fprintf(stderr, "Failed to load resources\n");
        return 0;
    }
    aff->highlight_texture = SDL_CreateTexture(aff->renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    SDL_SetRenderTarget(aff->renderer, aff->highlight_texture);
    SDL_SetRenderDrawColor(aff->renderer, 255, 255, 255, 255);
    SDL_RenderClear(aff->renderer);
    SDL_SetRenderTarget(aff->renderer, NULL);
    
    aff->map_rect.x = 0;
    aff->map_rect.y = 0;
    aff->map_rect.w = state->width * TILE_SIZE;
    aff->map_rect.h = state->height * TILE_SIZE;
    aff->mouse_rect.w = 2;
    aff->mouse_rect.h = 2;
    
    display_update_map(aff, state);
    return 1;
}

void display_update_map(Affichage* aff, EtatJeu* state) {
    aff->map_texture = SDL_CreateTexture(aff->renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        state->width * TILE_SIZE, state->height * TILE_SIZE);
    
    SDL_SetRenderTarget(aff->renderer,aff->map_texture);
    SDL_SetRenderDrawColor(aff->renderer, 0, 0, 0, 255);
    SDL_RenderClear(aff->renderer);
    
    for (int y = 0; y < state->height; y++) {
        for (int x = 0; x < state->width; x++) {
            if (state->tiles[y][x] == WALL) {
                SDL_SetRenderDrawColor(aff->renderer, 0, 0, 255, 255);
                SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderFillRect(aff->renderer, &rect);
            }
            if (state->tiles[y][x] == PATH){
                SDL_SetRenderDrawColor(aff->renderer, 255, 255, 0, 255);
                SDL_Rect rect = {x * TILE_SIZE + 15, y * TILE_SIZE + 15, 5, 5};
                SDL_RenderFillRect(aff->renderer, &rect);
            }
        }
    }
    SDL_SetRenderTarget(aff->renderer, NULL);
}

void display_set_mouse_position(Affichage* aff, int x, int y) {
    aff->mouse_rect.x = x;
    aff->mouse_rect.y = y;
}

void display_render(Affichage* aff, EtatJeu* state) {
    if (aff->pacman_texture) SDL_DestroyTexture(aff->pacman_texture);
    switch (state->move_direction) {
        case SDLK_UP:    
            aff->pacman_texture = IMG_LoadTexture(aff->renderer, "images/pacman-up.png");    
            break;
        case SDLK_DOWN:  
            aff->pacman_texture = IMG_LoadTexture(aff->renderer, "images/pacman-down.png");  
            break;
        case SDLK_LEFT:  
            aff->pacman_texture = IMG_LoadTexture(aff->renderer, "images/pacman-left.png");  
            break;
        case SDLK_RIGHT: 
            aff->pacman_texture = IMG_LoadTexture(aff->renderer, "images/pacman-right.png"); 
            break;
        default:         
            aff->pacman_texture = IMG_LoadTexture(aff->renderer, "images/pacman-full.png");  
            break;
    }
    SDL_Rect pacman_rect = { state->pacman_x, state->pacman_y, TILE_SIZE, TILE_SIZE };
    
    SDL_RenderClear(aff->renderer);
    SDL_RenderCopy(aff->renderer, aff->map_texture, NULL, &aff->map_rect);

    SDL_SetRenderDrawColor(aff->renderer, 0, 0, 0, 255);
    for (int y = 0; y < state->height; y++) {
        for (int x = 0; x < state->width; x++) {
            if (state->tiles[y][x] == PASSED) {
                SDL_Rect rect = {x * TILE_SIZE + 15, y * TILE_SIZE + 15, 10, 10};
                SDL_RenderFillRect(aff->renderer, &rect);
            }
        }
    }
    SDL_RenderCopy(aff->renderer, aff->pacman_texture, NULL, &pacman_rect);

    for (int i = 0; i < NB_GHOSTS; i++) {
        if (state->ghosts[i].active){
            SDL_Rect ghost_rect = { state->ghosts[i].x, state->ghosts[i].y, TILE_SIZE, TILE_SIZE };
            if (state->vulnerable) {
                SDL_RenderCopy(aff->renderer, aff->ghost_dizzy_texture, NULL, &ghost_rect);
            } else {
                int idx = direction_to_index(state->ghosts[i].direction);
                SDL_RenderCopy(aff->renderer, aff->ghost_textures[i][idx], NULL, &ghost_rect);
            }
        }
    }
    if (state->is_paused) {
        SDL_RenderCopy(aff->renderer, aff->highlight_texture, NULL, &aff->mouse_rect);
        
        char str[128];
        sprintf(str, " Mouse: (%d,%d) Pacman: (%d,%d)",
                aff->mouse_rect.x, aff->mouse_rect.y,
                state->pacman_x, state->pacman_y);
        
        SDL_Surface* surface = TTF_RenderText_Solid(aff->font, str,
            (SDL_Color){255, 255, 255, 0});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(aff->renderer, surface);
        
        SDL_Rect text_pos = {0, 0, surface->w, surface->h};
        SDL_RenderCopy(aff->renderer, texture, NULL, &text_pos);
        
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    SDL_RenderPresent(aff->renderer);
}

void display_cleanup(Affichage* aff) {
    if (aff->pacman_texture) SDL_DestroyTexture(aff->pacman_texture);

    for (int i = 0; i < NB_GHOSTS; i++){
        for (int j = 0; j < 4; j++){
            if (aff->ghost_textures[i][j]) SDL_DestroyTexture(aff->ghost_textures[i][j]);
        }
    }
    if (aff->map_texture) SDL_DestroyTexture(aff->map_texture);
    if (aff->highlight_texture) SDL_DestroyTexture(aff->highlight_texture);
    if (aff->ghost_dizzy_texture) SDL_DestroyTexture(aff->ghost_dizzy_texture);
    if (aff->font) TTF_CloseFont(aff->font);
    if (aff->renderer) SDL_DestroyRenderer(aff->renderer);
    if (aff->window) SDL_DestroyWindow(aff->window);
    TTF_Quit();
    SDL_Quit();
}