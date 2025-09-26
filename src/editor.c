
/*
    World selection                                                         ~
        in editor or/and in menu? Know which worlds already exist   
    zoom in/out, move                                                       v
    World selection                                                         ~
        in editor or/and in menu? Know which worlds already exist   
    zoom in/out, move                                                       v
        scale, viewpoint
    wall movement
        mouse click detection
    wall addition
        keyboard/button
    save button
        writer to world.txt
    random world generator
        wave func collapse


*/

#define EDIT_MENU (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT

static v2_f viewpoint = {0,0};
static float scale = 10.0f;


void render_world(struct World* world) {
    for (int i =0; i<world->wall_amount; i++) {
        struct Wall w= world->walls[i];
        SDL_SetRenderDrawColor(renderer,w.r,w.g,w.b,255);
        SDL_RenderDrawLineF(renderer,
                (w.v1.x-viewpoint.x)*scale,(w.v1.y-viewpoint.y)*scale,
                (w.v2.x-viewpoint.x)*scale,(w.v2.y-viewpoint.y)*scale
            );
    }
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderDrawPointF(renderer,(world->spawn.x-viewpoint.x) * scale,(world->spawn.y-viewpoint.y)*scale);
}


int handle_editor_button_press(int x, int y) {
    int menu[4] = {EDIT_MENU};
    return (x >= menu[0] && y >= menu[1] && x <= menu[0]+menu[2] && y <= menu[1]+menu[3]);
}

/*
    W/A/S/D             for moving around
    I/O                 for zooming (I)n/(O)ut
    n                   for new wall
    r                   for select next wall
    x                   for delete selected wall
    1                   for select vertex 1
    2                   for select vertex 2
    UP/DOWN/LEFT/RIGHT  for moving selected vertex (should scale with scale)

    save button during pause screen

    snap to grid function?
*/
int editor_loop() {
    SDL_Event e;
    int mouse_x = WIDTH/2;
    int mouse_y = HEIGHT/2;
    int ticks   = 0;
    int dticks  = 0;
    int pause   = 0;
    struct World* world = world_list->worlds[1]; // TODO : world selection
    while (1){
        //main game loop
        ticks = SDL_GetTicks();
        while(SDL_PollEvent(&e) !=0) {
            if (e.type == SDL_QUIT) return QUIT_MODE;
            //check if any button was clicked
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && pause) {
                SDL_GetMouseState(&mouse_x,&mouse_y);
                if (handle_editor_button_press(mouse_x, mouse_y)) return MENU_MODE;
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
                        if (scale == 0.0) {
                            scale = EPSILON;
                        }
                        break;
                    case SDLK_p:
                        pause ^= 1; //flip the pause flag
                    default :
                        break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //draw buttons
        if (pause) {
            SDL_SetRenderDrawColor( renderer, 0,0,0, 100 );
            const SDL_Rect whole_screen =   {0,0,WIDTH,HEIGHT};
            const SDL_Rect rect =           {EDIT_MENU};
            SDL_RenderFillRect(renderer,&whole_screen);
            SDL_SetRenderDrawColor( renderer,0,0,255,255);
            SDL_RenderFillRect(renderer,&rect);
            //draw button text
            render_button(menu_letters,4,&rect);
        } 
        render_world(world);

        SDL_RenderPresent(renderer);

        dticks = SDL_GetTicks() - ticks;
        dticks = 1000/ FPS - dticks;
        if (dticks >0) SDL_Delay(dticks);
    }
    return QUIT_MODE;
}