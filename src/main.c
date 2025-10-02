#include    <math.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <stdint.h>
#include    <string.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


/*
        TODO:
    save button for editor



*/



SDL_Window *window              = NULL;
SDL_Renderer *renderer          = NULL;
SDL_Surface* surface            = NULL;
SDL_Texture* spriteSheet        = NULL;
struct World_List* world_list   = NULL;

typedef _Bool   bool;
typedef float   f32;
typedef int32_t i32;

#define RENDER_DIST     100
#define FPS             30.0f

#define EDITOR_MODE     0
#define GAME_MODE       1
#define MENU_MODE       2
#define QUIT_MODE       3

#define HEIGHT          560
#define WIDTH           1000
#define BUTTON_WIDTH    200
#define BUTTON_HEIGHT   100
#define FONT_SIZE       32

#define MAX_WALLS           100
#define MAX_WORLDS          10
#define MAX_WORLD_NAME_LEN  50

#define RED             255,0,0,255
#define GREEN           0,255,0,255
#define BLUE            0,0,255,255
#define BLACK           0,0,0,255
#define WHITE           255,255,255,255
#define SHADOW          0,0,0,100
#define SKY_COLOR       0,0,64,255
#define FLOOR_COLOR     64,64,64,255

const i32 game_letters[4]   = {6,0,12,4};
const i32 editor_letters[6] = {4,3,8,19,14,17};
const i32 menu_letters[4]   = {12,4,13,20};
const i32 quit_letters[4]   = {16,20,8,19};

i32 selected_world = 0;

#include "utils/ray_casting.c"
#include "utils/display.c"
#include "world_loader.c"
#include "editor.c"
#include "game.c"
#include "menu.c"


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
void createRenderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "An error occured when initializing the renderer: %s\n", SDL_GetError());
        exit(1);
    }
    if (SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND) != 0) {
        fprintf(stderr, "An error occured when initializing the renderer: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        exit(1);
    }
}
void init_images() {
    i32 imgflags = IMG_INIT_PNG;
    if (!(IMG_Init(imgflags) & imgflags)) {
        fprintf(stderr, "Couldnt initialize image loader: %s\n",IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        exit(1);
    }
    surface =  IMG_Load("../resources/font.png");
    if (surface == NULL) {
        fprintf(stderr, "Couldnt load spritesheet: %s\n",IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        exit(1);
    }
}


int main() {
    //SDL initialization
    SDL_Init(SDL_INIT_VIDEO);
    createWindow();
    createRenderer(window);
    init_images();
    //Data&File loading
    spriteSheet = SDL_CreateTextureFromSurface(renderer,surface);
    world_list = load_world_file();
    i32 mode            = MENU_MODE;
    // Main process loop. Each screen returns the next screen to render or quit
    while (mode != QUIT_MODE) {
	
        if (mode == GAME_MODE)      {mode = game_loop();   continue;}
        if (mode == EDITOR_MODE)    {mode = editor_loop(); continue;}
        if (mode == MENU_MODE)      {mode = menu_loop();}
    }

    //freeing data & SDL exiting
    destroy_world_list(world_list);
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
