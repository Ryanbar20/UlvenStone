
/*
    World selection
        in editor or/and in menu? Know which worlds already exist
    zoom in/out, move
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

static v2_f viewpoint = {0,0};
static float scale = 10.0f;


void render_world(struct World* world) {
    for (int i =0; i<world->wall_amount; i++) {
        struct Wall w= world->walls[i];
        SDL_SetRenderDrawColor(renderer,w.r,w.g,w.b,255);
        SDL_RenderDrawLine(renderer,
                ((int) w.v1.x-viewpoint.x)*scale,((int) w.v1.y-viewpoint.y)*scale,
                ((int) w.v2.x-viewpoint.x)*scale,((int) w.v2.y-viewpoint.y)*scale
            );
    }
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderDrawPoint(renderer,(int) world->spawn.x,(int) world->spawn.y);
}


/*
    WASD for moving around
    I/O for zooming (I)n/(O)ut

*/
int editor_loop() {
    SDL_Event e;
    int ticks   = 0;
    int dticks  = 0;
    int pause   = 0;
    struct World* world = world_list->worlds[1];
    printf("%d\n",world->wall_amount);
    while (1){
        //main game loop
        ticks = SDL_GetTicks();
        while(SDL_PollEvent(&e) !=0) {
            if (e.type == SDL_QUIT) return QUIT_MODE;
            //check if any button was clicked
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                    case SDLK_w:
                        viewpoint.y += 10;
                        break;
                    case SDLK_s:
                        viewpoint.y -= 10;
                        break;
                    case SDLK_a:
                        viewpoint.x += 10;
                        break;
                    case SDLK_d:
                        viewpoint.x -= 10;
                        break;
                    case SDLK_i:
                        scale       += 1;
                        break;
                    case SDLK_o:
                        scale       -= 1;
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
        
        render_world(world);

        SDL_RenderPresent(renderer);

        dticks = SDL_GetTicks() - ticks;
        dticks = 1000/ FPS - dticks;
        if (dticks >0) SDL_Delay(dticks);
    }
    return QUIT_MODE;
}