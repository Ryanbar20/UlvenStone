

#define WALL_HEIGHT 100.0f
#define PLAYER_HEIGHT 50.0f
#define CAM_HEIGHT 50.0f
#define SCREEN_DIST 1   // corresponds to FOV (higher means less FOV)
#define CAM_WIDTH 1.5f  // corresponds to FOV (higher means more FOV) (currently around 85deg)




v2_f view;
v2_f pos;

#define SKY_COLOR 0,0,64,255
#define FLOOR_COLOR 64,64,64,255
#define GAME_MENU (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT


v2_f get_column(float distance) {
    int y_lo = (int) (HEIGHT/2.0f - ((PLAYER_HEIGHT/distance) * (HEIGHT / CAM_HEIGHT)) );
    int y_hi = (int) (HEIGHT/2.0f + (((WALL_HEIGHT-PLAYER_HEIGHT)/distance * (HEIGHT / CAM_HEIGHT))));
    y_lo = y_lo < 0 ? 0 : y_lo;
    y_hi = y_hi >= HEIGHT ? HEIGHT-1 : y_hi;
    return (v2_f) {HEIGHT-y_hi,HEIGHT-y_lo};
}

static v2_f get_ray(int screen_index,v2_f perpendicular) {
    const float dw = (CAM_WIDTH / 2.0f - ((CAM_WIDTH * screen_index)/WIDTH));
    return set_length(1,(v2_f) {dw*perpendicular.x + SCREEN_DIST*  view.x,dw*perpendicular.y + SCREEN_DIST* view.y});
}


void cast_rays(SDL_Renderer* renderer,struct World* world) {
    //declare loop variables
    struct Wall* wall_hit;  float wall_hit_distance;    v2_f ray;
    //constant static variables
    const v2_f perpendicular = rotate(DEG_TO_RAD(90),view);
    for (int i =0; i<WIDTH; i++) {
        //assign loop variables
        wall_hit = NULL;    wall_hit_distance = RENDER_DIST + 10;   ray = get_ray(i,perpendicular);

        //determine wall to draw, if any
        for (int j=0; j<world->wall_amount; j++) {
            //variables for this iteration
            const struct Wall w = world->walls[j];
            const float d = check_hit(ray,pos,w.v1, w.v2);
            if (d >= 0 && d < wall_hit_distance) {
                wall_hit = world->walls + j;    wall_hit_distance = d;
            }
        }

        //skip drawing stage
        if (wall_hit == NULL) {
            continue;
        };
        //create pixel boundaries for column
        float cos_angle = (ray.x*view.x + ray.y*view.y) / (get_length(ray) * get_length(view));
        const v2_f column = get_column(wall_hit_distance*cos_angle);

        //draw column with corresponding wall color
        SDL_SetRenderDrawColor(renderer,wall_hit->r,wall_hit->g,wall_hit->b,255);
        SDL_RenderDrawLine(renderer,i,column.y,i,column.x);

    }
}



int handle_game_button_press(int x, int y) {
    int menu[4] = {GAME_MENU};
    return (x >= menu[0] && y >= menu[1] && x <= menu[0]+menu[2] && y <= menu[1]+menu[3]);
}


int game_loop(SDL_Renderer* renderer, SDL_Window* window, struct World* world) {

    view    = set_length(1, (v2_f) {0,1});
    pos     = world->spawn;
    printf("%f %f\n",pos.x,pos.y);
    SDL_Event e;
    int quit    = 0;
    int mouse_x = WIDTH/2;
    int mouse_y = HEIGHT/2;
    int pause   = 0;
    int ticks   = 0;
    int dticks  = 0;
    while (!quit){
        //main game loop
        ticks = SDL_GetTicks();
        while(SDL_PollEvent(&e) !=0){
            if (e.type == SDL_QUIT) {
                return QUIT_MODE;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                switch (e.button.button) {
                    case SDL_BUTTON_LEFT:
                        if (pause) {
                            SDL_GetMouseState(&mouse_x,&mouse_y);
                            if (handle_game_button_press(mouse_x, mouse_y)) return MENU_MODE;
                            break;
                        }

                    default:
                        break;
                }
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                    pos.x += view.x;
                    pos.y += view.y;
                    break;
                case SDLK_s:
                    pos.x -= view.x;
                    pos.y -= view.y;
                    break;
                case SDLK_a:
                    pos.x -= view.y;
                    pos.y += view.x;
                    break;
                case SDLK_d:
                    pos.x += view.y;
                    pos.y -= view.x;
                    break;
                case SDLK_p:
                    pause ^= 1; //flip the pause flag
                default :
                    break;
                }
            }
            if (!pause) {
                SDL_GetMouseState(&mouse_x,&mouse_y);
                if (mouse_x < (WIDTH/2)) {
                    view = rotate(0.15,view);
                } else if (mouse_x > (WIDTH/2)) {
                    view = rotate(-0.15,view);
                }
                SDL_WarpMouseInWindow(window,WIDTH/2,HEIGHT/2);
            }


        }

        SDL_RenderClear( renderer );
        //rendering
        SDL_SetRenderDrawColor(renderer,SKY_COLOR);
        SDL_RenderFillRect(renderer,&((SDL_Rect) {0,0,WIDTH,HEIGHT/2}));
        SDL_SetRenderDrawColor(renderer,FLOOR_COLOR);
        SDL_RenderFillRect(renderer,&((SDL_Rect) {0,HEIGHT/2,WIDTH,(HEIGHT/2)}));

        cast_rays(renderer,world);
        if (pause) {
            SDL_SetRenderDrawColor( renderer, 0,0,0, 100 );
            const SDL_Rect whole_screen = {0,0,WIDTH,HEIGHT};
            SDL_RenderFillRect(renderer,&whole_screen);
            SDL_Rect rect = {GAME_MENU};
            SDL_SetRenderDrawColor( renderer,0,0,255,255);
            SDL_RenderFillRect(renderer,&rect);

        }
        SDL_RenderPresent( renderer );

        dticks = SDL_GetTicks() - ticks; // total ticks the frame took sofar
        dticks = 1000/ FPS - dticks;    // ticks that are left to wait for next frame
        if (dticks >0) {
            printf("%d\n",dticks);
            SDL_Delay(dticks);
        }

    }
    return QUIT_MODE;

}