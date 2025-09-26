void render_button(const int* letters,int letter_amount,const SDL_Rect* button) {
    for (int i = 0; i < letter_amount; i++) {
        v2_f letter_xy = get_sprite_coordinate(letters[i]);
        SDL_Rect srcRect = (SDL_Rect) {letter_xy.x, letter_xy.y,8,8};
        SDL_Rect dstRect = (SDL_Rect) {button->x+FONT_SIZE*i+i,
            button->y+(button->h - FONT_SIZE)/2,FONT_SIZE,FONT_SIZE};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);
    }
}
