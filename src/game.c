

#define WALL_HEIGHT     100.0f
#define PLAYER_HEIGHT   50.0f
#define CAM_HEIGHT      50.0f
#define SCREEN_DIST     1   // corresponds to FOV (higher means less FOV)
#define CAM_WIDTH       1.5f  // corresponds to FOV (higher means more FOV) (currently around 85deg)

#define GAME_MENU   (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT


v2_f view;
v2_f pos;





inline v2_f get_column(f32 distance) {
    i32 y_lo    = (i32) (HEIGHT/2.0f - ((PLAYER_HEIGHT/distance) * (HEIGHT / CAM_HEIGHT)) );
    i32 y_hi    = (i32) (HEIGHT/2.0f + (((WALL_HEIGHT-PLAYER_HEIGHT)/distance * (HEIGHT / CAM_HEIGHT))));
    y_lo        = y_lo < 0 ? 0 : y_lo;
    y_hi        = y_hi >= HEIGHT ? HEIGHT-1 : y_hi;
    return      (v2_f) {HEIGHT-y_hi,HEIGHT-y_lo};
}

inline v2_f get_ray(i32 screen_index,v2_f perp) {
    const f32 dw = (CAM_WIDTH / 2.0f - ((CAM_WIDTH * screen_index)/WIDTH));
    return set_length(1,(v2_f) {dw*perp.x + SCREEN_DIST*  view.x,dw*perp.y + SCREEN_DIST* view.y});
}


void cast_rays(const struct World* world) {
    const v2_f perpendicular        = rotate(DEG_TO_RAD(90),view);
    for (i32 i =0; i<WIDTH; i++) {
        //assign loop variables
        struct Wall *wall_hit       = NULL;
        f32 wall_hit_distance       = RENDER_DIST + 10;
        v2_f ray                    = get_ray(i, perpendicular);

        //determine wall to draw, if any
        for (i32 j=0; j<world->wall_amount; j++) {
            //variables for this iteration
            const struct Wall w     = world->walls[j];
            const f32 d             = check_hit(ray,pos,w.v1, w.v2,RENDER_DIST);
            if (d >= 0 && d < wall_hit_distance) {
                wall_hit            = world->walls + j;
                wall_hit_distance   = d;
            }
        }

        //skip drawing stage
        if (wall_hit == NULL) continue;
        //create pixel boundaries for column
        f32 cos_angle       = (ray.x*view.x + ray.y*view.y) / (get_length(ray) * get_length(view));
        const v2_f column   = get_column(wall_hit_distance*cos_angle);

        //draw column with corresponding wall color
        SDL_SetRenderDrawColor(renderer,wall_hit->r,wall_hit->g,wall_hit->b,255);
        SDL_RenderDrawLine(renderer,i,column.y,i,column.x);

    }
}

void move_if_possible(v2_f* pos, const v2_f movement, const struct World* world) {
    for (i32 i=0; i<world->wall_amount; i++) {
        const struct Wall w = world->walls[i];
        if (check_hit(movement,*pos,w.v1,w.v2,1) >0) {
            return;
        }
    }
    pos->x += movement.x;
    pos->y += movement.y;
}




i32 handle_game_button_press(i32 x, i32 y) {
    i32 menu[4] = {GAME_MENU};
    return (x >= menu[0] && y >= menu[1] && x <= menu[0]+menu[2] && y <= menu[1]+menu[3]);
}

i32 game_loop() {

    struct World* world = world_list->worlds[selected_world];
    view        = set_length(1, (v2_f) {0,1});
    pos         = world->spawn;

    
    i32 quit    = 0;
    i32 pause   = 0;

    SDL_Event e;
    i32 mouse_x = WIDTH/2;
    i32 mouse_y = HEIGHT/2;

    i32 ticks   = 0;
    i32 dticks  = 0;
    
    while (!quit){
        //main game loop
        ticks   = SDL_GetTicks();
        while(SDL_PollEvent(&e) !=0){

            if (e.type == SDL_QUIT) return QUIT_MODE;
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && pause) {
                SDL_GetMouseState(&mouse_x,&mouse_y);
                if (handle_game_button_press(mouse_x, mouse_y)) return MENU_MODE;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                    case SDLK_w:
                        move_if_possible(&pos,(v2_f) {view.x,view.y}, world);
                        break;
                    case SDLK_s:
                        move_if_possible(&pos,(v2_f) {-view.x,-view.y}, world);
                        break;
                    case SDLK_a:
                        move_if_possible(&pos,(v2_f) {-view.y,view.x}, world);
                        break;
                    case SDLK_d:
                        move_if_possible(&pos,(v2_f) {view.y,-view.x}, world);
                        break;
                    case SDLK_p:
                        pause ^= 1; //flip the pause flag
                        break;
                    default :
                        break;
                }
            }
        }
        if (!pause) {
            SDL_GetMouseState(&mouse_x,&mouse_y);
            if      (mouse_x < WIDTH/2) {view =  rotate(0.15,view);}
            else if (mouse_x > WIDTH/2) {view =  rotate(-0.15,view);}
            SDL_WarpMouseInWindow(window,WIDTH/2,HEIGHT/2);
        }


        SDL_RenderClear( renderer );
        //rendering
        SDL_SetRenderDrawColor(renderer,SKY_COLOR);
        SDL_RenderFillRect(renderer, &((SDL_Rect) {0,0,WIDTH,HEIGHT/2}));
        SDL_SetRenderDrawColor(renderer,FLOOR_COLOR);
        SDL_RenderFillRect(renderer, &((SDL_Rect) {0,HEIGHT/2,WIDTH,(HEIGHT/2)}));

        cast_rays(world);
        if (pause) {
            
            const SDL_Rect whole_screen =   {0,0,WIDTH,HEIGHT};
            SDL_SetRenderDrawColor( renderer, SHADOW);
            SDL_RenderFillRect(renderer,&whole_screen);

            const SDL_Rect rect =           {GAME_MENU};
            render_button(menu_letters,4,&rect, BLUE);
        }
        SDL_RenderPresent( renderer );

        dticks = 1000/ FPS - (SDL_GetTicks() - ticks);
        printf("%d\n", dticks);
        if (dticks >0) SDL_Delay(dticks);
    }
    return QUIT_MODE;

}