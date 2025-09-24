#include    <math.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#define HEIGHT  560
#define WIDTH   1000

SDL_Window *window          = NULL;
SDL_Renderer *renderer      = NULL;
SDL_Surface* surface        = NULL;
SDL_Texture* spriteSheet    = NULL;

#define RENDER_DIST     100
#define FPS             30.0f

#define MENU_MODE       2
#define GAME_MODE       1
#define EDITOR_MODE     0
#define QUIT_MODE       3


#define BUTTON_WIDTH    200
#define BUTTON_HEIGHT   100
#define FONT_SIZE       32


const int game_letters[4]   = {6,0,12,4};
const int editor_letters[6] = {4,3,8,19,14,17};
const int menu_letters[4]   = {12,4,13,20};
const int quit_letters[4]   = {16,20,8,19};

#include "utils.c"
#include "world_loader.c"
#include "editor.c"
#include "game.c"
#include "menu.c"




/*
 *  Todo:
    Make world editor
 * 
 */


//function that initializes an SDL window
//  also checks for correct initialization
void createWindow() {
    window = SDL_CreateWindow(
        "UlvenStone",
        SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,0
    );
    if (window == NULL) {
        fprintf(stderr, "An error occured when creating the window: %s\n", SDL_GetError());
        exit(1);
    }
}

//function that initializes an SDL renderer
//  also checks for correct initialization
void createRenderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "An error occured when initializing the renderer: %s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND) != 0) {
        fprintf(stderr, "An error occured when initializing the renderer: %s\n", SDL_GetError());
        exit(1);
    }
}
void init_images() {
    int imgflags = IMG_INIT_PNG;
    if (!(IMG_Init(imgflags) & imgflags)) {
        fprintf(stderr, "Couldnt initialize image loader: %s\n",IMG_GetError());
        exit(1);
    }
    surface =  IMG_Load("../resources/font.png");
    if (surface == NULL) {
        fprintf(stderr, "Couldnt load spritesheet: %s\n",IMG_GetError());
        exit(1);
    }
}

//function that handles the main program loop, inputs and cleanup after closing.
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    createWindow();
    createRenderer(window);
    init_images();
    spriteSheet = SDL_CreateTextureFromSurface(renderer,surface);

    struct World* world = load_world();
    int mode            = MENU_MODE;

    while (mode != QUIT_MODE) {
        if (mode == GAME_MODE)      {mode = game_loop(world);   continue;}
        if (mode == EDITOR_MODE)    {mode = editor_loop(world); continue;}
        if (mode == MENU_MODE)      {mode = menu_loop(renderer);}
    }

    //free data
    free(world->walls);
    free(world);
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}