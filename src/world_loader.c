



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
  char** names; // names of the worlds
  struct World** worlds; // array containing all the worlds in the world_file
};



struct World* load_world(char* world_name) {
    FILE* file = fopen("../resources/world.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error while opening the world.txt file\n");
        return NULL;
    }
    char buffer[256];
    bool found = 0;
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
    bool found_spawn = 0;
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
        fclose(file);
        return NULL;
    }
    struct World_names* names = malloc(sizeof(struct World_names));
    if (!names) {
        fprintf(stderr, "Error while allocating memory\n");
        fclose(file);
        return NULL;
    }
    names->world_amt = num_world; names->names = world_names;
    fclose(file);
    return names;
}


struct World_List* load_world_file() {
    /*
        Loads the world.txt file into a World_List struct
    */
    struct World_names* names = check_world_file_syntax();
    if (!names) return NULL;
    struct World_List* file = (struct World_List*)malloc(sizeof(struct World_List));
    if (!file) goto delete_file;
    struct World** worlds = malloc(MAX_WORLDS * sizeof(struct World*));
    if (!worlds) goto delete_worlds;
    char** names_list = (char**)malloc(MAX_WORLDS * sizeof(char*));
    if (!names_list) goto delete_names_list;

    for (i32 i =0; i < names->world_amt; i++) {
        struct World* world = load_world(names->names + i * MAX_WORLD_NAME_LEN);
        if (world == NULL) goto delete_all;
        worlds[i] = world;
        // names is a blob of bytes, thus freeing them per name doesnt work ...
        char* name = (char*)malloc(sizeof(char) * MAX_WORLD_NAME_LEN);
        if (name == NULL) {
            //handle malloc failed
        }
        strncpy(name,(names->names + i * sizeof(char) *  MAX_WORLD_NAME_LEN), MAX_WORLD_NAME_LEN);
        names_list[i] = name;
    }
    file->world_amt = names->world_amt;
    file->names = names_list;
    file->worlds = worlds;
    free(names->names);
    free(names);
    return file;
    

    delete_all:
        free(names_list);
    delete_names_list:
        free(worlds);
    delete_worlds:
        free(file);
    delete_file:
        free(names->names);
        free(names);
    return NULL;
}


void destroy_world_list(struct World_List* list) {
    for (i32 i = 0; i < list->world_amt; i++) {
        free(list->worlds[i]->walls);
        free(list->worlds[i]);
        free(list->names[i]);
    }
    free(list->names);
    free(list->worlds);
    free(list);
}



i32 world_exists(const char* name,const struct World_List* list) {
    for (i32 i =0; i<list->world_amt;i++) {
        if (strncmp(name,list->names[i],MAX_WORLD_NAME_LEN) ==0) {
            return i;
        }
    }
    return -1;
}


struct World_List* save_world(struct World* world,  char* const name) {
    /*
        saves the world by deleting the contents of world.txt and rewriting them

        It first performs checks, then loads the current file and adds World world to it
        Then it writes the new list to the world.txt file by first emptying it out completely

        Returns a World_List containing all worlds (updated)
    */
    if (strnlen(name,MAX_WORLD_NAME_LEN) == MAX_WORLD_NAME_LEN) return NULL;
    struct World_List* list = load_world_file();
    if (list == NULL) return NULL;


    i32 new_world_added = world_exists(name, list);
    if (new_world_added != -1) {

        struct Wall* wall_copy = (struct Wall*)malloc(sizeof(struct Wall) * MAX_WALLS);
        if (wall_copy == NULL) {
            destroy_world_list(list);
            return NULL;
        }
        struct World* world_copy = (struct World*)malloc(sizeof(struct World));
        if (world_copy == NULL) {
            destroy_world_list(list);
            return NULL;
        }
        memcpy(wall_copy,world->walls,sizeof(struct Wall) * MAX_WALLS);
        memcpy(world_copy,world,sizeof(struct World));
        world_copy->walls = wall_copy;

        list->worlds[new_world_added] =world_copy;
        char* name_cpy = malloc(MAX_WORLD_NAME_LEN);
        if (name_cpy == NULL) {
            destroy_world_list(list);
            return NULL;
        }
        strncpy(name_cpy,name,MAX_WORLD_NAME_LEN);
        list->names[new_world_added] = name_cpy;


        new_world_added = 0;
    } else {
        list->names[list->world_amt] = name;   // IF THIS IS NOT MALLOCED -> ERROR
        list->worlds[list->world_amt] = world; // IF THIS IS NOT MALLOCED -> ERROR
        new_world_added = 1;
    }
    list->world_amt +=new_world_added;
    FILE* world_txt = fopen("../resources/world.txt", "w+");
    if (world_txt == NULL) {
        destroy_world_list(list);
        return NULL;
    }
    for (i32 i = 0; i < list->world_amt; i++) {
        struct World* current_world = list->worlds[i];
        fprintf(world_txt, "%s\n",list->names[i]);
        fprintf(world_txt, "S %f %f\n", current_world->spawn.x,current_world->spawn.y);
        for (i32 j = 0; j < current_world->wall_amount; j++) {
            struct Wall cur_wall = current_world->walls[j];
            fprintf(world_txt,"%f %f %f %f %d %d %d\n",cur_wall.v1.x,cur_wall.v1.y,
                            cur_wall.v2.x,cur_wall.v2.y,cur_wall.r,cur_wall.g,cur_wall.b );
        }
        fprintf(world_txt, "END\n");
    }
    fclose(world_txt);
    return list;
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
