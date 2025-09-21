#include    <stdio.h>
#include    <math.h>
#include    <stdlib.h>
#include    <string.h>
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define HEIGHT  560
#define WIDTH   1000
#define RENDER_DIST 100
#define WALL_HEIGHT 0.1f

#define CAM_HEIGHT  0.01f
#define SCREEN_DIST 1   // corresponds to FOV (higher means less FOV)
#define CAM_WIDTH 1.5f  // corresponds to FOV (higher means more FOV) (currently around 85deg)
#include "utils.c"
#include "world_loader.c"

v2_f view;
v2_f pos;

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


v2_f get_column(float distance) {
    int y_lo = (int) (HEIGHT/2.0f - CAM_HEIGHT/distance * HEIGHT / CAM_HEIGHT);
    int y_hi = (int) (HEIGHT/2.0f - (WALL_HEIGHT-CAM_HEIGHT/distance * HEIGHT / CAM_HEIGHT));
    y_lo = y_lo < 0 ? 0 : y_lo;
    y_hi = y_hi >= HEIGHT ? HEIGHT-1 : y_hi;
    return (v2_f) {y_lo, y_hi};
}

static inline v2_f get_ray(int screen_index,v2_f perpendicular) {
    float dw = (CAM_WIDTH / 2.0f - ((CAM_WIDTH * screen_index)/WIDTH));
    return set_length(1,(v2_f) {dw*perpendicular.x + view.x,dw*perpendicular.y + view.y});
}


void cast_rays(SDL_Renderer* renderer,struct World* world) {
    //declare loop variables
    struct Wall* wall_hit;  float wall_hit_distance;    v2_f ray;
    //constant static variables
    const v2_f perpendicular = rotate(DEG_TO_RAD(90),view);
    for (int i =0; i<WIDTH; i++) {
        //assign loop variables
        wall_hit = NULL;    wall_hit_distance = RENDER_DIST + 10;   ray = get_ray(i,perpendicular);
        //constant variables
        const float angle = get_angle_between_vectors(ray,view);
        
        //determine wall to draw, if any
        for (int j=0; j<world->wall_amount; j++) {
            //variables for this iteration
            const struct Wall w = world->walls[j];
            const float d = check_hit(ray,pos,w.v1, w.v2);
            if (d >= 0 && d < wall_hit_distance) {
                wall_hit = world->walls + j;    wall_hit_distance = d;
            }
        }

        //skip drawing stage if no wall has been hit
        if (wall_hit == NULL) continue;
        //create pixel boundaries for column
        const v2_f column = get_column(wall_hit_distance*cos(angle));

        //draw column with corresponding wall color
        SDL_SetRenderDrawColor(renderer,wall_hit->r,wall_hit->g,wall_hit->b,255);
        SDL_RenderDrawLine(renderer,i,column.x,i,column.y);

    }   
}



//function that handles the main program loop, inputs and cleanup after closing.
int main() {

    struct World* world = load_world();
    print_world(world);
    print_world_layout(world);

    view    = set_length(1, (v2_f) {0.5,2.5});
    pos     = (v2_f) {0.5,0.5};
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
                    view = rotate(DEG_TO_RAD(5.0),view);
                    break;
                case SDLK_RIGHT:
                    view = rotate(DEG_TO_RAD(-5.0),view);
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