#include    <stdio.h>
#include    <math.h>
#include    <stdlib.h>
#include    <string.h>
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define HEIGHT  560
#define WIDTH   1000


#define RENDER_DIST 100
#define FPS 30.0f

#define MENU_MODE 2
#define GAME_MODE 1
#define EDITOR_MODE 0
#define QUIT_MODE 3

#include "utils.c"
#include "world_loader.c"
#include "game.c"
#include "editor.c"
#include "menu.c"


/*
 *  Todo:
    Make world editor
 * 
 */


//function that initializes an SDL window
//  also checks for correct initialization
SDL_Window* createWindow() {
    SDL_Window* window = SDL_CreateWindow(
        "UlvenStone",
        SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,0
    );
    if (window == NULL) {
        fprintf(stderr, "An error occured when creating the window: %s\n", SDL_GetError());
    }
    return window;
}

//function that initializes an SDL renderer
//  also checks for correct initialization
SDL_Renderer* createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "An error occured when initializing the renderer: %s\n", SDL_GetError());
    }
    if (SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND) != 0) {
        fprintf(stderr, "An error occured when initializing the renderer: %s\n", SDL_GetError());
    }   
    return renderer;
}







//function that handles the main program loop, inputs and cleanup after closing.
int main() {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window =        createWindow();
    SDL_Renderer *renderer =    createRenderer(window);
    struct World* world = load_world();
    int mode = MENU_MODE;

    while (mode != QUIT_MODE) {
        if (mode == GAME_MODE) {
            mode = game_loop(renderer,window,world);
        } else if (mode == EDITOR_MODE) {
            mode = editor_loop(renderer,window,world);
        } else if (mode == MENU_MODE) {
            mode = menu_loop(renderer,window);
        }
    }


    //free data
    free(world->walls);
    free(world);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}