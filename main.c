#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algo.h"
#include "affichage.h"
#include "generate_lab.h"


int main(int argc, char** argv) {
    EtatJeu state= {0};
    Affichage display= {0};

    char generated_map[] = "generated_map.txt";

    const char* map_file = NULL;
    if (argc > 1) {
        if (strcmp(argv[1], "-r") == 0) {
            int cells_w = 10;
            int cells_h = 10;
            
            if (argc > 2) {
                cells_w = atoi(argv[2]);
                if (argc > 3) {
                    cells_h = atoi(argv[3]);
                } else {
                    cells_h = cells_w;
                }
                if (cells_w < 5){
                    cells_w = 5;
                }
                if (cells_h < 5){
                    cells_h = 5;
                }
            }
            if (create_map_txt(generated_map, cells_w, cells_h)) {
                map_file = generated_map;
            }
        } else {
            map_file = argv[1];
        }
    }
    game_init(&state, map_file);
    fprintf(stdout, "Score : \n");
    
    if (!display_init(&display, &state)) {
        game_cleanup(&state);
        return EXIT_FAILURE;
    }
    int is_running = 1;
    srand(time(NULL));
    
    while (is_running) {
        SDL_Event event;
        SDL_PollEvent(&event);
        
        switch (event.type) {
            case SDL_QUIT:
                is_running = 0;
                break;
            case SDL_MOUSEMOTION:
                display_set_mouse_position(&display,
                    event.button.x / RENDER_SCALE,
                    event.button.y / RENDER_SCALE);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        is_running = 0;
                        break;
                    case SDLK_r:
                        if (state.game_over){
                            game_init(&state, map_file);
                            display_update_map(&display, &state);
                            state.move_direction = 0;
                            state.want_direction = 0;
                        }
                        break;
                    case SDLK_p:
                        state.is_paused = !state.is_paused;
                        break;
                    case SDLK_UP:
                    case SDLK_DOWN:
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                        if (!state.is_paused && !state.game_over) {
                            state.want_direction = event.key.keysym.sym;
                        }
                        break;
                }
                break;
        }
        game_update(&state);
        display_render(&display, &state);
        SDL_Delay(2);
    }
    display_cleanup(&display);
    game_cleanup(&state);
    return EXIT_SUCCESS;
}