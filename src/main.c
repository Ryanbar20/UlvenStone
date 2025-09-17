#include    <stdio.h>
#include    <stdlib.h>
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

//720p
#define HEIGHT  720
#define WIDTH   1280


#include "world_loader.c"

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
    return renderer;
}

//function that handles the main program loop, inputs and cleanup after closing.
int main() {

    load_world();
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = createWindow();
    //SDL_Renderer *renderer = createRenderer(window);
    SDL_Event e;
    int quit = 0;
    while (!quit){
        //main game loop
        while(SDL_PollEvent(&e) !=0){
            if (e.type == SDL_QUIT) {
                quit=1;
                continue;
            }
        }
    }

    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}