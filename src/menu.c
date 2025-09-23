


#define GAME_OFFSET 200
#define EDITOR_RECT (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT
#define GAME_RECT (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - (BUTTON_HEIGHT / 2) + GAME_OFFSET, BUTTON_WIDTH, BUTTON_HEIGHT

int handle_menu_button_press(int x, int y) {
    int editor[4] = {EDITOR_RECT};
    int game[4] = {GAME_RECT};
    if (x >= editor[0] && y >= editor[1] && x <= editor[0]+editor[2] && y <= editor[1]+editor[3]) {
        return GAME_MODE;
    }
    if (x >= game[0] && y >= game[1] && x <= game[0]+game[2] && y <= game[1]+game[3]) {
        return EDITOR_MODE;
    }
    return MENU_MODE;
}

int menu_loop(SDL_Renderer* renderer) {
    //TODO
    SDL_Event e;
    int quit = 0;
    int mouse_x; int mouse_y;
    int ticks   = 0;
    int dticks  = 0;
    while (!quit){
        //main game loop
        ticks = SDL_GetTicks();
        while(SDL_PollEvent(&e) !=0) {
            switch (e.type) {
                case SDL_QUIT:
                    return QUIT_MODE;
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            SDL_GetMouseState(&mouse_x,&mouse_y);
                            mouse_x = handle_menu_button_press(mouse_x, mouse_y);
                            if (mouse_x != MENU_MODE) {
                                return mouse_x;
                            }
                            break;
                        default:
                            break;

                    }
                    break;
                default:
                    break;

            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect editor = {EDITOR_RECT};
        SDL_Rect game = {GAME_RECT};
        SDL_RenderFillRect(renderer,&game);
        SDL_RenderFillRect(renderer, &editor);
        SDL_RenderPresent(renderer);

        dticks = SDL_GetTicks() - ticks;
        dticks = 1000/ FPS - dticks;
        if (dticks >0) {
            SDL_Delay(dticks);
        }
    }
    return QUIT_MODE;


}