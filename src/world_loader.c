



#define WALL 0
#define END 1
#define NAME 2
#define SPAWN 3
#define WALL_OR_END 4


struct Wall{
    v2_f v1, v2;
    i32 r,g,b;
};


struct World{
    i32 wall_amount;
    v2_f spawn;
    struct Wall* walls;
};


struct World_names{
    i32 world_amt;
    char* names; // points to char[MAX_WORLDS][MAX_WORLD_NAME_LEN]
};

struct World_List{
  i32 world_amt; // amount of worlds in the world file
  struct World_names* names; // names of the worlds
  struct World** worlds; // array containing all the worlds in the world_file
};



struct World* load_world(char* world_name) {
    FILE* file = fopen("../resources/world.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error while opening the world.txt file\n");
        return NULL;
    }
    char buffer[256];
    i32 found = 0;
    //find world
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(world_name,buffer) == 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "Invalid world name passed: %s\n",world_name);
        goto file_error;
    }

    struct Wall* walls = (struct Wall*)malloc(MAX_WALLS * sizeof(struct Wall));
    if (!walls) {
        fprintf(stderr, "Memory Allocation Failed in world_loader\n");
        goto file_error;
    }
    i32 count = 0;
    f32 x1, y1, x2, y2; 
    i32 r, g, b;
    char t;
    i32 found_spawn = 0;
    v2_f spawn;
    // read world
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // if we reached the end of this world declaration
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp("END",buffer) == 0)  break;

        if (buffer[0] == 'S') {
            if (sscanf(buffer, "%c %f %f",&t,&x1,&y1) != 3) goto load_error;
            found_spawn = 1;
            spawn.x     = x1;
            spawn.y     = y1;
            continue;
        }

        if (sscanf(buffer, "%f %f %f %f %d %d %d",&x1,&y1,&x2,&y2,&r,&g,&b) != 7) goto load_error;
        if (!found_spawn) goto load_error;


        walls[count++] = (struct Wall) {{x1,y1},{x2,y2},r,g,b};
    }
    struct World* world = (struct World*)malloc(sizeof(struct World));
    if (!world) {
        printf("Memory allocation failed for World\n");
        goto world_malloc_error;
    }
    
    world->wall_amount  = count;
    world->spawn        = spawn;
    world->walls        = walls;
    fclose(file);
    return world;

    load_error:
        fprintf(stderr,"Load error on: %s\n",buffer);
    world_malloc_error:
        free(walls);
    file_error:
        fclose(file);
        return NULL;
}




struct World_names* check_world_file_syntax() {
    /*
        Returns a pointer to a char[MAX_WORLDS][MAX_WORLD_NAME_LEN] containing all world names
    */
    FILE* file = fopen("../resources/world.txt", "r");
    if (!file) {
        fprintf(stderr, "Error while opening the world.txt file\n");
        return NULL;
    }
    char buffer[256];
    char* world_names = malloc(sizeof(char) * MAX_WORLDS * MAX_WORLD_NAME_LEN) ;
    i32 num_world = 0;
    i32 line =-1;
    i32 next_token_expected = NAME;
    char c; 
    f32 t1, t2, t3, t4;
    i32 t5, t6, t7;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line++;
        if (buffer[0] == 'E'&& buffer[1] =='N'&&  buffer[2] =='D'&& buffer[3] =='\n' && (next_token_expected == END || next_token_expected == WALL_OR_END)) {
            next_token_expected = NAME;
            continue;
        }
        if (sscanf(buffer, "%c %f %f",&c,&t1,&t2) == 3 && c == 'S' && next_token_expected == SPAWN) {
            next_token_expected = WALL;
            continue;
        }
        if (sscanf(buffer, "%f %f %f %f %d %d %d",&t1,&t2,&t3,&t4,&t5,&t6,&t7) == 7 && (next_token_expected == WALL || next_token_expected == WALL_OR_END)) {
            next_token_expected = WALL_OR_END;
            continue;
        }
        if (next_token_expected == NAME) {
            buffer[strcspn(buffer, "\n")] = '\0';   // strcspn also works when the name is the only line (no \n visible that is)
            strcpy(world_names + num_world * MAX_WORLD_NAME_LEN,buffer);
            num_world++;
            next_token_expected = SPAWN;
            continue;
        }
        fprintf(stderr, "Syntax error in world.txt at line %d: %s",line, buffer);
        return NULL;
    }

    // char* temp = (char*)realloc(world_names,sizeof(char) * MAX_WORLD_NAME_LEN * num_world);
    // if (temp != NULL) world_names = temp;
    struct World_names* names = malloc(sizeof(struct World_names));
    if (!names) {
        fprintf(stderr, "Error while allocating memory\n");
        return NULL;
    }
    names->world_amt = num_world; names->names = world_names;
    return names;
}


struct World_List* load_world_file() {
    struct World_names* names = check_world_file_syntax();
    if (!names) return NULL;
    struct World_List* file = (struct World_List*)malloc(sizeof(struct World_List));
    if (!file) goto delete_file;
    struct World** worlds = malloc(names->world_amt * sizeof(struct World*));
    if (!worlds) goto delete_worlds;

    for (i32 i =0; i < names->world_amt; i++) {
        char* name = names->names + i * MAX_WORLD_NAME_LEN;
        struct World* world = load_world(name);
        if (world == NULL) return NULL;
        worlds[i] = world;
    }
    file->world_amt = names->world_amt;
    file->names = names;
    file->worlds = worlds;
    return file;

    delete_worlds:
        free(file);
    delete_file:
        free(names->names);
        free(names);
    return NULL;
}


void destroy_world_list(struct World_List* list) {
    free(list->names->names);
    free(list->names);
    for (i32 i = 0; i < list->world_amt; i++) {
        free(list->worlds[i]);
    }
    free(list->worlds);
    free(list);
}



void print_world_layout(const struct World* world) {
    printf("Wall Amount: %d\n", world->wall_amount);
    i32 maxy = 0; i32 miny = 0;i32 maxx =0;i32 minx =0;
    //get world dimensions
    for (i32 i =0; i<world->wall_amount;i++) {
        struct Wall w = world->walls[i];
        if (w.v1.x > maxx)        {maxx = w.v1.x;}
        else if (w.v1.x <minx)    {minx = w.v1.x;}
        if (w.v2.x > maxx)        {maxx = w.v2.x;}
        else if (w.v2.x <minx)    {minx = w.v2.x;}
        if (w.v1.y > maxy)        {maxy = w.v1.y;}
        else if (w.v1.y <miny)    {miny = w.v1.y;}
        if (w.v2.y > maxy)        {maxy = w.v2.y;}
        else if (w.v2.y <miny)    {miny = w.v2.y;}
    }
    
    //get char[][] containing world layout
    char layout[maxy-miny+1][maxx-minx+1];
    memset(layout, '-',sizeof(layout));
    for (i32 i =0; i<world->wall_amount;i++) {
        struct Wall w = world->walls[i];
        layout[(i32) w.v1.y-miny][(i32) w.v1.x-minx] = '#';
        layout[(i32) w.v2.y-miny][(i32) w.v2.x-minx] = '#';
    }

    for (i32 y = maxy-miny; y>=0; y--) {
        for (i32 x = 0; x<=(maxx-minx);x++) {
            printf("%c",layout[y][x]);
        }
        printf("\n");
    }


}
