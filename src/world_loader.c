#define MAX_WALLS 100



#define WALL 0
#define END 1
#define NAME 2
#define SPAWN 3
#define WALL_OR_END 4


/*
    TODO:
    handle world choice
*/
struct Wall{
    v2_f v1;
    v2_f v2;
    int r;
    int g;
    int b;
};


struct World{
    int wall_amount;
    v2_f spawn;
    struct Wall* walls;
};


struct World_names{
    int world_amt;
    char* names; // points to char[MAX_WORLDS][MAX_WORLD_NAME_LEN]
};

struct World_List{
  int world_amt; // amount of worlds in the world file
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
    int found = 0;
    //find world
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(world_name,buffer) == 0) {
            found = 1;
            break;
        }
    }
    // check if world found
    if (found == 0) {
        fprintf(stderr, "Invalid world name passed: %s\n",*world_name);
        return NULL;
    }

    struct Wall* walls = (struct Wall*)malloc(MAX_WALLS * sizeof(struct Wall));
    int count = 0;
    float x1; float y1; float x2; float y2; int r; int g; int b;
    char t;
    int found_spawn = 0;
    v2_f spawn;
    // read world
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // if we reached the end of this world declaration
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp("END",buffer) == 0) {
            break;
        }
        if (buffer[0] == 'S') {
            found_spawn = 1;
            if (sscanf(buffer, "%c %f %f",&t,&x1,&y1) != 3) {
                fprintf(stderr,"Parse error on: %s\n", buffer);
                free(walls);
                return NULL;
            }
            spawn.x = x1;
            spawn.y = y1;
            continue;
        }

        if (sscanf(buffer, "%f %f %f %f %d %d %d",&x1,&y1,&x2,&y2,&r,&g,&b) != 7) {
            fprintf(stderr,"Parse error on: %s\n", buffer);
            free(walls);
            return NULL;
        }
        if (!found_spawn) {
            fprintf(stderr,"Parse error on: %s\n Spawn should be specified before any walls", buffer);
            free(walls);
            return NULL;
        }
        struct Wall w = {{x1,y1},{x2,y2},r,g,b};
        walls[count] = w;
        count++; 
    }

    struct Wall* temp = (struct Wall*)realloc(walls, count*sizeof(struct Wall));
    if (temp == NULL) {
        printf("Reallocation failed!\n");
    } else {
        walls = temp;       
    }
    struct World* world = (struct World*)malloc(sizeof(struct World));
    if (world == NULL) {
        printf("Memory allocation failed for World\n");
        free(walls);
        return NULL;
    }
    world->wall_amount = count;
    world->spawn = spawn;
    world->walls = walls;
    return world;
}

// prints the wall count and walls of the given world
void print_world(struct World* world){
    printf("Wall Amount: %d\n", world->wall_amount);

    for (int i = 0; i<world->wall_amount;i++) {
        struct Wall w = world->walls[i];
        printf("%d %d %d %d %d %d %d\n",(int) w.v1.x,(int) w.v1.y,(int) w.v2.x,(int) w.v2.y,(int) w.r,w.g,w.b);
    }
}


void print_world_layout(struct World* world) {
    printf("Wall Amount: %d\n", world->wall_amount);
    int maxy = 0; int miny = 0;int maxx =0;int minx =0;
    //get world dimensions
    for (int i =0; i<world->wall_amount;i++) {
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
    for (int i =0; i<world->wall_amount;i++) {
        struct Wall w = world->walls[i];
        layout[(int) w.v1.y-miny][(int) w.v1.x-minx] = '#';
        layout[(int) w.v2.y-miny][(int) w.v2.x-minx] = '#';
    }

    for (int y = maxy-miny; y>=0; y--) {
        for (int x = 0; x<=(maxx-minx);x++) {
            printf("%c",layout[y][x]);
        }
        printf("\n");
    }


}



struct World_names* check_world_file_syntax() {
    /*
        Returns a pointer to a char[MAX_WORLDS][MAX_WORLD_NAME_LEN] containing all world names

    
    */
    FILE* file = fopen("../resources/world.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error while opening the world.txt file\n");
        return NULL;
    }
    char buffer[256];
    char* world_names = malloc(sizeof(char) * MAX_WORLDS * MAX_WORLD_NAME_LEN) ;
    int num_world = 0;
    int line =-1;
    int next_token_expected = NAME;
    char c; 
    float t1; float t2; float t3; float t4;int t5;int t6;int t7;

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
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(world_names + num_world * MAX_WORLD_NAME_LEN,buffer);
            num_world++;
            next_token_expected = SPAWN;
            continue;
        }
            fprintf(stderr, "Syntax error in world.txt at line %d: %s",line, buffer);
            return NULL;
        }

    char* temp = (char*)realloc(world_names,sizeof(char) * MAX_WORLD_NAME_LEN * num_world);
    if (temp != NULL) world_names = temp;
    struct World_names* names = malloc(sizeof(struct World_names));
    names->world_amt = num_world; names->names = world_names;
    return names;
}


struct World_List* load_world_file() {
    struct World_names* names = check_world_file_syntax();
    struct World_List* file = (struct World_List*)malloc(sizeof(struct World_List));
    struct World* worlds[names->world_amt];
    for (int i =0; i < names->world_amt; i++) {
        char* name = names->names + i * MAX_WORLD_NAME_LEN;
        struct World* world = load_world(name);
        worlds[i] = world;
    }
    file->world_amt = names->world_amt;
    file->names = names;
    file->worlds = &worlds[0];
    return file;
}


void destroy_world_list(struct World_List* list) {
    free(list->names->names);
    for (int i = 0; i < list->world_amt; i++) {
        free(list->worlds[i]);
    }
}