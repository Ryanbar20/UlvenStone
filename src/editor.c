


#define EDIT_MENU (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT
#define EDIT_SAVE (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - BUTTON_HEIGHT / 2 + 150, BUTTON_WIDTH, BUTTON_HEIGHT   
#define SAVE_FLAG -1

v2_f viewpoint      = {0,0};
f32 scale           = 10.0f;
i32 selected_wall   = 0;
i32 selected_vertex = 0; //0 or 1

void render_world(struct World* world) {
    for (i32 i =0; i<world->wall_amount; i++) {
        struct Wall w= world->walls[i];
        SDL_SetRenderDrawColor(renderer,w.r,w.g,w.b,255);
        if (i == selected_wall) {
            SDL_SetRenderDrawColor(renderer,WHITE);
            SDL_Rect r;
            if (selected_vertex ==0 ) {
                r = (SDL_Rect) {(w.v1.x-viewpoint.x)*scale - 5,(w.v1.y-viewpoint.y)*scale -5,10,10};
            } else {
                r = (SDL_Rect) {(w.v2.x-viewpoint.x)*scale - 5,(w.v2.y-viewpoint.y)*scale -5,10,10};
            }
            
            SDL_RenderFillRect(renderer, &r);
            
        }
        SDL_RenderDrawLineF(renderer,
                (w.v1.x-viewpoint.x)*scale,(w.v1.y-viewpoint.y)*scale,
                (w.v2.x-viewpoint.x)*scale,(w.v2.y-viewpoint.y)*scale
            );
    }
    SDL_SetRenderDrawColor(renderer,BLACK);
    SDL_RenderDrawPointF(renderer,(world->spawn.x-viewpoint.x) * scale,(world->spawn.y-viewpoint.y)*scale);
}


i32 handle_editor_button_press(i32 x, i32 y) {
    i32 menu[4] = {EDIT_MENU};
    if (x >= menu[0] && y >= menu[1] && x <= menu[0]+menu[2] && y <= menu[1]+menu[3]) {
        return MENU_MODE;
    }
    i32 save[4] = {EDIT_SAVE};
    if (x >= save[0] && y >= save[1] && x <= save[0]+save[2] && y <= save[1]+save[3]) {
        return SAVE_FLAG;
    }
    return EDITOR_MODE;
}


void move_vertex(f32 dx, f32 dy,struct World* w) {

    if (selected_vertex == 0) {
        w->walls[selected_wall].v1.x += dx;
        w->walls[selected_wall].v1.y += dy;
        return;
    }
    w->walls[selected_wall].v2.x += dx;
    w->walls[selected_wall].v2.y += dy;
    return;
}

/*
    W/A/S/D             for moving around               x
    I/O                 for zooming (I)n/(O)ut          x
    n/b                 for next/previous wall          x
    r                   for new wall
    x                   for delete selected wall
    1                   for select vertex 1             x
    2                   for select vertex 2             x
    h/j/k/l (LDUR)      for moving selected vertex      x

    save button during pause screen                     x

    snap to grid function?
*/
i32 editor_loop() {
    SDL_Event e;
    i32 mouse_x = WIDTH/2;
    i32 mouse_y = HEIGHT/2;
    i32 ticks   = 0;
    i32 dticks  = 0;
    i32 pause   = 0;
    struct World* world;
    struct Wall* walls = NULL;

    // declare world
    if (selected_world >= world_list->world_amt) {
        world = (struct World*)malloc(sizeof(struct World));
        if (!world) return QUIT_MODE;
        walls = (struct Wall*)malloc(MAX_WALLS * sizeof(struct Wall));
        if (!walls) {
            free(world);
            return QUIT_MODE;
        }
        world->wall_amount = 0;
        world->walls = walls;
    } else {
        world = world_list->worlds[selected_world];
    }

     
    while (1){
        //main game loop
        ticks = SDL_GetTicks();
        while(SDL_PollEvent(&e) !=0) {
            if (e.type == SDL_QUIT) {
                if (!walls) {
                    free(world);
                    free(walls);
                }
                return QUIT_MODE;
            }
            //check if any button was clicked
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && pause) {
                SDL_GetMouseState(&mouse_x,&mouse_y);
                mouse_x= handle_editor_button_press(mouse_x, mouse_y);
                if (mouse_x == MENU_MODE) {
                    if (walls != NULL) {
                        free(world);
                        free(walls);
                    }
                    return MENU_MODE;
                }
                if (mouse_x == SAVE_FLAG) {
                    struct World_List* new_list = NULL;
                    if (selected_world < world_list->world_amt) {
                        new_list = save_world(world,world_list->names[selected_world]);
                        if ( new_list == NULL) {
                            printf("Couldnt save world\n");
                        };
                    } else {
                        // add an option to enter a name for your world
                        // MAKE SURE THE STRING NAME IS MALLOCED
                    }
                    printf("saved\n");
                    destroy_world_list(world_list);
                    world_list = new_list;
                    if (world_list == NULL) {
                        printf("error while saving\n");
                    }
                    return MENU_MODE;
                    
                }
                
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                    case SDLK_w:
                        viewpoint.y -= 5/scale;
                        break;
                    case SDLK_s:
                        viewpoint.y += 5/scale;
                        break;
                    case SDLK_a:
                        viewpoint.x -= 5/scale;
                        break;
                    case SDLK_d:
                        viewpoint.x += 5/scale;
                        break;
                    case SDLK_i:
                        scale       /= 0.9;
                        break;
                    case SDLK_o:
                        scale       *= 0.9;
                        if (scale == 0.0) scale = EPSILON;
                        break;
                    case SDLK_n:
                        if (world->wall_amount == 0) break;
                        selected_wall = (selected_wall + 1) % world->wall_amount;
                        break;
                    case SDLK_b:
                        if (world->wall_amount == 0) break;
                        selected_wall = (selected_wall+world->wall_amount - 1) % world->wall_amount;
                        break;
                    case SDLK_r:
                        if (world->wall_amount == MAX_WALLS) {
                            printf("Max Walls reached\n");
                            break;
                        }
                        world->walls[world->wall_amount] = (struct Wall) {(v2_f) {-1,0},(v2_f) {1,0}, 255,0,255};
                        world->wall_amount++;
                        break;
                    case SDLK_1:
                        selected_vertex = 0;
                        break;
                    case SDLK_2:
                        selected_vertex = 1;
                        break;
                    case SDLK_h:
                        move_vertex(-0.2,0,world);
                        break;
                    case SDLK_j:
                        move_vertex(0,-0.2,world);
                        break;
                    case SDLK_k:
                        move_vertex(0,0.2,world);
                        break;
                    case SDLK_l:
                        move_vertex(0.2,0,world);
                        break;
                    case SDLK_p:
                        pause ^= 1; //flip the pause flag
                        break;
                    default :
                        break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //draw buttons
        if (pause) {
            
            const SDL_Rect whole_screen =   {0,0,WIDTH,HEIGHT};
            SDL_SetRenderDrawColor( renderer, SHADOW);
            SDL_RenderFillRect(renderer,&whole_screen);

            SDL_Rect rect =                 {EDIT_MENU};
            SDL_SetRenderDrawColor( renderer,BLUE);
            SDL_RenderFillRect(renderer,&rect);
            render_button(menu_letters,4,&rect);

            rect =             (SDL_Rect)   {EDIT_SAVE};
            SDL_SetRenderDrawColor( renderer,RED);
            SDL_RenderFillRect(renderer,&rect);
            
            const i32 save[4] = {18,0,21,4};
            render_button(&save[0],4, &rect);
        } 

        render_world(world);
        SDL_RenderPresent(renderer);

        dticks = 1000/ FPS - (SDL_GetTicks() - ticks);
        if (dticks >0) SDL_Delay(dticks);
    }
    if (walls != NULL) {
        free(world);
        free(walls);
    }
    return MENU_MODE;
        
}