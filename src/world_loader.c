#define MAX_WALLS 100
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


// for now, default world is CUBE
struct World* load_world() {
    FILE* file = fopen("../resources/world.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error while opening the world.txt file\n");
        return NULL;
    }
    char name[] = "blouseWorld";
    char buffer[256];
    int found = 0;
    //find world
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(name,buffer) == 0) {
            found = 1;
            break;
        }
    }
    // check if world found
    if (found == 0) {
        fprintf(stderr, "Invalid world name passed: %s\n",name);
        return NULL;
    }

    struct Wall* walls = (struct Wall*)malloc(MAX_WALLS * sizeof(struct Wall));
    int count = 0;
    int x1; int y1; int x2; int y2; int r; int g; int b;
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
            if (sscanf(buffer, "%c %d %d",&t,&x1,&y1) != 3) {
                printf("Parse error on: %s\n", buffer);
                free(walls);
                return NULL;
            }
            spawn.x = x1;
            spawn.y = y1;
            continue;
        }

        if (sscanf(buffer, "%d %d %d %d %d %d %d",&x1,&y1,&x2,&y2,&r,&g,&b) != 7) {
            printf("Parse error on: %s\n", buffer);
            free(walls);
            return NULL;
        }
        if (!found_spawn) {
            printf("Parse error on: %s\n Spawn should be specified before any walls", buffer);
            free(walls);
            return NULL;
        }
        struct Wall w = {{x1,y1},{x2,y2},r,g,b};
        walls[count] = w;
        count++; 
    }

    printf("%d\n",count);
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