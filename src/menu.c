


#define GAME_OFFSET                 200
#define QUIT_RECT                   100, (HEIGHT / 2) - (BUTTON_HEIGHT / 2) + GAME_OFFSET,   BUTTON_WIDTH, BUTTON_HEIGHT
#define EDITOR_RECT                 100, (HEIGHT / 2) - (BUTTON_HEIGHT / 2),                 BUTTON_WIDTH, BUTTON_HEIGHT
#define GAME_RECT                   100, (HEIGHT / 2) - (BUTTON_HEIGHT / 2) - GAME_OFFSET,   BUTTON_WIDTH, BUTTON_HEIGHT
#define WORLD_OUT_OF_BOUNDS_RECT    400,400,400,20
#define WORLD_NAME_RECT             350, 100,400,10
#define INPUT_RECT                  WIDTH / 10, HEIGHT / 10, WIDTH * 0.8, HEIGHT *0.8
#define WORLD_NAME_RECT_Y_OFFSET    15

i32 handle_menu_button_press(i32  x,i32  y) {
    const i32 editor[4] = {EDITOR_RECT};
    if (x >= editor[0] && y >= editor[1] && x <= editor[0]+editor[2] && y <= editor[1]+editor[3]) {
        return EDITOR_MODE;
    }
    const i32 game[4]   = {GAME_RECT};
    if (x >= game[0] && y >= game[1] && x <= game[0]+game[2] && y <= game[1]+game[3]) {
        return GAME_MODE;
    }
    const i32 quit[4]   = {QUIT_RECT};
    if (x >= quit[0] && y >= quit[1] && x <= quit[0]+quit[2] && y <= quit[1]+quit[3]) {
        return QUIT_MODE;
    }
    return MENU_MODE;
}

i32 key_to_digit(SDL_Keycode key) {
    if (key >= SDLK_0 && key <= SDLK_9) return key - SDLK_0;
    return -1;
}

char key_to_char(SDL_Keycode key) {
    // add 97 to return to make it the correct CHAR value
    if (key >= SDLK_a && key <= SDLK_z) return key - SDLK_a + 97;
    return -1;
}


i32 menu_loop() {
    
    bool reading_new_world_name = 0;
    i32 new_world_name_len = 0;         // if reading_new_world_name = 0, this should always be 0
    char* new_world_name = NULL;        // if reading_new_world_name = 0, this should always be NULL

    i32 display_world_out_of_bounds = 0;
    SDL_Event e;
    i32 mouse_x; 
    i32 mouse_y;
    i32 ticks   = 0;
    i32 dticks  = 0;
    while (1){
        ticks = SDL_GetTicks();
        while(SDL_PollEvent(&e) !=0) {
            if (e.type == SDL_QUIT) return QUIT_MODE;
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                SDL_GetMouseState(&mouse_x,&mouse_y);
                switch (handle_menu_button_press(mouse_x, mouse_y)) {

                    case GAME_MODE:
                        if (selected_world < world_list->world_amt) return GAME_MODE;
                        display_world_out_of_bounds = 1;
                        break;

                    case EDITOR_MODE:
                        if (selected_world < world_list->world_amt) return EDITOR_MODE;
                        reading_new_world_name = 1;
                        new_world_name_len = 0;
                        new_world_name = (char*)malloc(sizeof(char) * MAX_WORLD_NAME_LEN);
                        if (new_world_name == NULL) return QUIT_MODE;
                        break;

                    case QUIT_MODE:
                        return QUIT_MODE;
                        break;

                    default:
                        break;

                }
            }
            if (e.type == SDL_KEYDOWN) {
                i32 k = key_to_digit(e.key.keysym.sym);
                if (k != -1) {
                    selected_world = k;
                    continue;
                }

                if (reading_new_world_name) {
                    char c = key_to_char(e.key.keysym.sym);
                    if ( c != -1) {
                        new_world_name[new_world_name_len++] = c;
                        continue;
                    }
                    if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                        struct World* world = (struct World*)malloc(sizeof(struct World));
                        struct Wall* walls  = (struct Wall*)malloc(MAX_WALLS * sizeof(struct Wall));
                        if (!world) goto destroy_world_name;
                        if (!walls) goto destroy_world;

                        new_world_name[new_world_name_len]  = '\0';
                        selected_world                      = world_list->world_amt;

                        world->wall_amount  = 0;
                        world->walls        = walls;
                        world->spawn        = (v2_f) {0.35,0.35};

                        world_list->names[world_list->world_amt]    = new_world_name;
                        world_list->worlds[world_list->world_amt]   = world;
                        world_list->world_amt++;

                        return EDITOR_MODE;


                        destroy_world:
                            free(world);
                        destroy_world_name:
                            free(new_world_name);
                        return QUIT_MODE;

                    }
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        {   // drawing buttons
            SDL_Rect quit       = {QUIT_RECT};
            SDL_Rect game       = {GAME_RECT};
            SDL_Rect editor     = {EDITOR_RECT};

            render_button(quit_letters,4,&quit,RED);
            render_button(game_letters,4,&game,BLUE);
            render_button(editor_letters,6,&editor,BLUE);
        }

        {   // drawing world n
            SDL_Rect world_rect = {WORLD_NAME_RECT};
            world_rect.y        -= WORLD_NAME_RECT_Y_OFFSET;
            for (i32  i =0; i < MAX_WORLDS; i++) {
                world_rect.y    += WORLD_NAME_RECT_Y_OFFSET;
                SDL_Color c = (i == selected_world) ?  (SDL_Color) {WHITE} :(SDL_Color) {BLUE};
                if (i >= world_list->world_amt) {
                    SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
                    SDL_RenderFillRect(renderer,&world_rect);
                } else {
                    render_string(world_list->names[i],&world_rect,c.r,c.g,c.b,c.a);
                }
            }
        }
        if (reading_new_world_name) {
            SDL_Rect input_rect = {INPUT_RECT};
            render_string("enter level name",&input_rect, BLUE);
        }

        if (display_world_out_of_bounds) {
            SDL_Rect error_rect = {WORLD_OUT_OF_BOUNDS_RECT};
            render_string("cannot start game in nonexistent world",&error_rect,RED);
        }
        SDL_RenderPresent(renderer);


        dticks = 1000/ FPS - (SDL_GetTicks() - ticks);
        if (dticks >0) SDL_Delay(dticks);
    }
    return QUIT_MODE;

}