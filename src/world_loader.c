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
        exit(1);
    }
    char name[] = "cube";
    char buffer[256];
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strcmp(name,buffer) == -1) {
            found = 1;
            continue;
        }
    }
    if (found == 0) {
        fprintf(stderr, "Invalid world name passed: %s\n",name);
    }

    return NULL;
}
