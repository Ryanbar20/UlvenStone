



v2_f get_sprite_coordinate(i32 sprite_id) {
    i32 y   = sprite_id / 8;
    i32 x   = sprite_id % 8;
    return  (v2_f) {x*8,y*8};
}



void render_button(const i32* letters,i32 letter_amount,const SDL_Rect* button) {
    for (i32 i = 0; i < letter_amount; i++) {
        v2_f letter_xy = get_sprite_coordinate(letters[i]);
        SDL_Rect srcRect = (SDL_Rect) {letter_xy.x, letter_xy.y,8,8};
        SDL_Rect dstRect = (SDL_Rect) {button->x+FONT_SIZE*i+i,
            button->y+(button->h - FONT_SIZE)/2,FONT_SIZE,FONT_SIZE};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);
    }
}

i32 char_to_index(char a) {
    i32 i = (i32) a;
    if (i >90) return i -97;
    return i - 65;
}

void render_string(const char* string, const SDL_Rect* rect) {
    i32 i = 0;
    while (1) {
        char c = *(string + i);
        if (c =='\0') break;
        i32 index = char_to_index(c);
        v2_f letter_xy = get_sprite_coordinate(index);
        SDL_Rect srcRect = (SDL_Rect) {letter_xy.x, letter_xy.y,8,8};
        SDL_Rect dstRect = (SDL_Rect) {rect->x + 9*i,rect->y+1,8,8};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);

        i++;
    }
}

