#include    <stdio.h>
#include    <math.h>
#include    <stdlib.h>
#include    <string.h>
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"


#include "utils.c"
#include "world_loader.c"

#define HEIGHT  560
#define WIDTH   1000

//game vars
#define FOV 100
#define RENDER_DIST 10
struct v2_f view;
struct v2_f pos;

/*
 *  Todo:
    Handle world spawn specialized to the world
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
    return renderer;
}



//cast 1 ray per degree of FOV
/*
    For angle of array:
        make the ray
        for wall in walls:
            check if ray hit
            if so, check if closest hit
        if a wall has been hit:
            draw wall to screen
*/
void cast_rays(SDL_Renderer* renderer,struct World* world) {
    for (int i =0; i<FOV; i++) {
        int angle = -FOV / 2 + i;
        struct v2_f ray = set_length(10,rotate(DEG_TO_RAD(angle),view));
        struct Wall* wall_hit = NULL;
        int wall_hit_distance = -100;

        for (int i=0; i<world->wall_amount; i++) {
            struct Wall w = world->walls[i];
            int d = check_hit(ray,pos,&w);
            if (d != -1 && d < wall_hit_distance) {
                wall_hit = &w;
                wall_hit_distance = d;
            }
        }
        if (wall_hit != NULL) {
            printf("HIT\n");
        }
        
    }



}



//function that handles the main program loop, inputs and cleanup after closing.
int main() {

    struct World* world = load_world();
    print_world(world);
    print_world_layout(world);

    view    = (struct v2_f) {0,1};
    pos     = (struct v2_f) {0,0};
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = createWindow();
    SDL_Renderer *renderer = createRenderer(window);
    cast_rays(renderer,world);
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

        //rendering
    }

    //free data
    free(world->walls);
    free(world);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}