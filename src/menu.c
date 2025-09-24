


#define GAME_OFFSET 200
#define EDITOR_RECT (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT
#define GAME_RECT (WIDTH / 2) - BUTTON_WIDTH / 2, (HEIGHT / 2) - (BUTTON_HEIGHT / 2) + GAME_OFFSET, BUTTON_WIDTH, BUTTON_HEIGHT

int handle_menu_button_press(int x, int y) {
    const int editor[4] = {EDITOR_RECT};
    const int game[4] = {GAME_RECT};
    if (x >= editor[0] && y >= editor[1] && x <= editor[0]+editor[2] && y <= editor[1]+editor[3]) {
        return EDITOR_MODE;
    }
    if (x >= game[0] && y >= game[1] && x <= game[0]+game[2] && y <= game[1]+game[3]) {
        return GAME_MODE;
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
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //draw buttons
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect editor     = {EDITOR_RECT};
        SDL_Rect game       = {GAME_RECT};
        SDL_RenderFillRect(renderer,&game);
        SDL_RenderFillRect(renderer, &editor);
        //draw button text
        for (int i = 0; i < 4; i++) {
            v2_f letter_xy = get_sprite_coordinate(game_letters[i]);
            SDL_Rect srcRect = (SDL_Rect) {letter_xy.x, letter_xy.y, 8,8};
            SDL_Rect dstRect = (SDL_Rect) {game.x+FONT_SIZE*(i+1)+1,
                game.y+(BUTTON_HEIGHT - FONT_SIZE)/2,FONT_SIZE,FONT_SIZE};
            SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);
        }
        for (int i = 0; i < 6; i++) {
            v2_f letter_xy = get_sprite_coordinate(editor_letters[i]);
            SDL_Rect srcRect = (SDL_Rect) {letter_xy.x, letter_xy.y,8,8};
            SDL_Rect dstRect = (SDL_Rect) {editor.x+FONT_SIZE*i+1,
                editor.y+(BUTTON_HEIGHT - FONT_SIZE)/2,FONT_SIZE,FONT_SIZE};
            SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);
        }

        SDL_RenderPresent(renderer);

        dticks = SDL_GetTicks() - ticks;
        dticks = 1000/ FPS - dticks;
        if (dticks >0) {
            SDL_Delay(dticks);
        }
    }
    return QUIT_MODE;


}