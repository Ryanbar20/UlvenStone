#include    <stdio.h>
#include    <math.h>
#include    <stdlib.h>
#include    <string.h>
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define HEIGHT  560
#define WIDTH   1000
#define FOV 100
#define RENDER_DIST 10
#define WALL_HEIGHT 100

#include "utils.c"
#include "world_loader.c"

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
    struct Wall* wall_hit;
    float wall_hit_distance;
    for (int i =0; i<WIDTH; i++) {
        float angle = -FOV / 2 + i * ((float) FOV) / ((float) WIDTH) ;
        struct v2_f ray = rotate(DEG_TO_RAD(angle),view);
        wall_hit = NULL;
        wall_hit_distance = RENDER_DIST + 10;

        for (int i=0; i<world->wall_amount; i++) {
            struct Wall w = world->walls[i];
            float d = check_hit(ray,pos,w.v1, w.v2);
            if (d >= 0 && d < wall_hit_distance) {
                wall_hit = world->walls + i;
                wall_hit_distance = d;
            }
        }
        if (wall_hit == NULL) {
            continue;
        }

        float projPlaneDist = ((float) WIDTH) / (2 * tan(DEG_TO_RAD(((float) FOV) / 2.0f)));
        float corrected_dist = wall_hit_distance  *cos(DEG_TO_RAD(angle));
        float wall_height = projPlaneDist * 0.25 / corrected_dist;
        SDL_Rect wall_piece = {
            i,
            (int)( (HEIGHT / 2) - (wall_height / 2) ),
            1,
            (int)( wall_height )
        };
        SDL_SetRenderDrawColor(renderer,wall_hit->r,wall_hit->g,wall_hit->b,255);
        SDL_RenderFillRect(renderer,&wall_piece);

    }


    
}



//function that handles the main program loop, inputs and cleanup after closing.
int main() {

    struct World* world = load_world();
    print_world(world);
    print_world_layout(world);

    view    = set_length(1, (struct v2_f) {-1,-1});
    pos     = (struct v2_f) {4.5,4.5};
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = createWindow();
    SDL_Renderer *renderer = createRenderer(window);
    
    
    SDL_Event e;
    int quit = 0;
    while (!quit){
        //main game loop
        while(SDL_PollEvent(&e) !=0){
            if (e.type == SDL_QUIT) {
                quit=1;
                continue;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    view = rotate(DEG_TO_RAD(-5.0),view);
                    break;
                case SDLK_RIGHT:
                    view = rotate(DEG_TO_RAD(5.0),view);
                    break;
                default :
                    break;
                }
            }
        }
        
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
        SDL_RenderClear( renderer );
        SDL_SetRenderDrawColor(renderer,0,0,64,255);
        SDL_RenderFillRect(renderer, &((SDL_Rect) {0,0,WIDTH,HEIGHT /2}));
        //rendering
        cast_rays(renderer,world);
        SDL_RenderPresent( renderer );
    }

    //free data
    free(world->walls);
    free(world);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}