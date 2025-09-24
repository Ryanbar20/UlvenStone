


#define GAME_OFFSET 200
#define QUIT_RECT (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - (BUTTON_HEIGHT / 2) - GAME_OFFSET, BUTTON_WIDTH, BUTTON_HEIGHT
#define EDITOR_RECT (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT
#define GAME_RECT (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - (BUTTON_HEIGHT / 2) + GAME_OFFSET, BUTTON_WIDTH, BUTTON_HEIGHT

int handle_menu_button_press(int x, int y) {
    const int editor[4] = {EDITOR_RECT};
    const int game[4] = {GAME_RECT};
    const int quit[4] = {QUIT_RECT};
    if (x >= editor[0] && y >= editor[1] && x <= editor[0]+editor[2] && y <= editor[1]+editor[3]) {
        return EDITOR_MODE;
    }
    if (x >= game[0] && y >= game[1] && x <= game[0]+game[2] && y <= game[1]+game[3]) {
        return GAME_MODE;
    }
    if (x >= quit[0] && y >= quit[1] && x <= quit[0]+quit[2] && y <= quit[1]+quit[3]) {
        return QUIT_MODE;
    }
    return MENU_MODE;
}

int menu_loop(SDL_Renderer* renderer) {
    //TODO
    SDL_Event e;
    int mouse_x; int mouse_y;
    int ticks   = 0;
    int dticks  = 0;
    while (1){
        //main game loop
        ticks = SDL_GetTicks();
        while(SDL_PollEvent(&e) !=0) {
            if (e.type == SDL_QUIT) return QUIT_MODE;
            //check if any button was clicked
            int condition   = e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT;
            SDL_GetMouseState(&mouse_x,&mouse_y);
            mouse_x         = handle_menu_button_press(mouse_x, mouse_y);
            if (condition && mouse_x == GAME_MODE)      return GAME_MODE;
            if (condition && mouse_x == EDITOR_MODE)    return EDITOR_MODE;
            if (condition && mouse_x == QUIT_MODE)      return QUIT_MODE;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //draw buttons
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect editor     = {EDITOR_RECT};
        SDL_Rect game       = {GAME_RECT};
        SDL_Rect quit       = {QUIT_RECT};
        SDL_RenderFillRect(renderer,&game);
        SDL_RenderFillRect(renderer, &editor);
        SDL_SetRenderDrawColor(renderer, 255, 0,0, 255);
        SDL_RenderFillRect(renderer,&quit);
        //draw button text
        render_button(editor_letters,6,&editor);
        render_button(game_letters,4,&game);
        render_button(quit_letters,4,&quit);

        SDL_RenderPresent(renderer);

        dticks = SDL_GetTicks() - ticks;
        dticks = 1000/ FPS - dticks;
        if (dticks >0) {
            SDL_Delay(dticks);
        }
    }
    return QUIT_MODE;


}