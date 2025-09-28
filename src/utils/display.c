



v2_f get_sprite_coordinate(int sprite_id) {
    int y   = sprite_id / 8;
    int x   = sprite_id % 8;
    return  (v2_f) {x*8,y*8};
}



void render_button(const int* letters,int letter_amount,const SDL_Rect* button) {
    for (int i = 0; i < letter_amount; i++) {
        v2_f letter_xy = get_sprite_coordinate(letters[i]);
        SDL_Rect srcRect = (SDL_Rect) {letter_xy.x, letter_xy.y,8,8};
        SDL_Rect dstRect = (SDL_Rect) {button->x+FONT_SIZE*i+i,
            button->y+(button->h - FONT_SIZE)/2,FONT_SIZE,FONT_SIZE};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);
    }
}

int char_to_index(char a) {
    int i = (int) a;
    if (i >90) {
        return i -97;
    }
    return i - 65;
}

void render_string(const char* string, const SDL_Rect* rect) {
    int i = 0;
    while (1) {
        char c = *(string + i);
        if (c =='\0') break;
        int index = char_to_index(c);
        v2_f letter_xy = get_sprite_coordinate(index);
        SDL_Rect srcRect = (SDL_Rect) {letter_xy.x, letter_xy.y,8,8};
        SDL_Rect dstRect = (SDL_Rect) {rect->x + 9*i,rect->y+1,8,8};
        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &dstRect);

        i++;
    }
}

