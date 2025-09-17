#define MAX_WALLS 100
/*
    TODO:
    read world.txt
    parse walls
    handle multiple worlds
    handle data allocation
*/
struct Wall{
    int x1;
    int y1;
    int x2;
    int y2;
    int r;
    int g;
    int b;
};


struct World{
    int wall_amount;
    struct Wall* walls;
};


// for now, default world is CUBE
struct World* load_world() {
    FILE* file = fopen("../resources/world.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error while opening the world.txt file\n");
        return NULL;
    }
    char name[] = "cube";
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
    // read world
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // if we reached the end of this world declaration
        if (strcmp("END",buffer) == -1) {
            break;
        }
        //malloc to max walls and then realloc
        if (sscanf(buffer, "%d %d %d %d %d %d %d",&x1,&y1,&x2,&y2,&r,&g,&b) != 7) {
            printf("Parse error on: %s\n", buffer);
            free(walls);
            return NULL;
        }
        struct Wall w = {x1,y1,x2,y2,r,g,b};
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
    world->walls = walls;
    return world;
}

// prints the wall count and walls of the given world
void print_world(struct World* world){
    printf("Wall Amount: %d\n", world->wall_amount);

    for (int i = 0; i<world->wall_amount;i++) {
        struct Wall w = world->walls[i];
        printf("%d %d %d %d %d %d %d\n",w.x1,w.y1,w.x2,w.y2,w.r,w.g,w.b);
    }
}