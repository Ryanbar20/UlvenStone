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
        if (strcmp(name,buffer) == -1) {
            found = 1;
            continue;
        }
    }
    // check if world found
    if (found == 0) {
        fprintf(stderr, "Invalid world name passed: %s\n",name);
        return NULL;
    }

    struct Wall* walls = (struct Wall*)malloc(MAX_WALLS * sizeof(struct Wall));
    int count = 0;
    // read world
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        //malloc to max walls and then realloc


        // if we reached the end of this world declaration
        if (strcmp("END",buffer) == -1) {
            break;
        }
    }

    struct Wall* temp = (struct Wall*)realloc(walls, count*sizeof(struct Wall));
    if (temp == NULL) {
        printf("Reallocation failed!\n");
    } else {
        walls = temp;       
    }
    

    printf()
    free(walls);
    return NULL;
}
