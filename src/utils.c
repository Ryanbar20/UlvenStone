
#define PI              3.14159265359
#define RAD_TO_DEG(x)   x * (180 / PI)
#define DEG_TO_RAD(x)   x * (PI / 180)



struct v2_f {
    float x;
    float y;
};

static inline float get_distance(struct v2_f v1, struct v2_f v2) {
    return sqrt((v1.x-v2.x)*(v1.x-v2.x)  +  (v1.y-v2.y)*(v1.y-v2.y));
}

static inline float get_length(struct v2_f vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

struct v2_f set_length(float len, struct v2_f vector) {
    float scale = len / get_length(vector);
    return (struct v2_f) {scale * vector.x, scale * vector.y};
}

static inline struct v2_f rotate(float rad, struct v2_f vector) {

    return (struct v2_f) {vector.x * cos(rad) - vector.y*sin(rad), vector.x * sin(rad) + vector.y*cos(rad)};
}

// returns -1 if not hit, else returns distance to hit
float check_hit(struct v2_f ray,struct v2_f position, struct v2_f w1, struct v2_f w2) {
    int v1_under;
    int v2_under;
    float a_ray = -1;
    float b_ray = -1;
    if (ray.x == 0) {
        v1_under = w1.x > position.x;
        v2_under = w2.x > position.x;
    } else {

        a_ray = ray.y / ray.x ;
        b_ray = position.y - a_ray * position.x;
        v1_under = w1.y < (w1.x * a_ray + b_ray);
        v2_under = w2.y < (w2.x* a_ray + b_ray);
    }
    if (v1_under == v2_under) { // if wall is parralel, it wont hit
        return (float) -1;
    }
    

    float hit_x;
    float hit_y;
    if (w1.x-w2.x ==0) {
        float dist = get_distance(w1,position);
        // dist2 is to check that the wall is in front of the player.
        float dist2 = get_distance(w1,set_length(RENDER_DIST,ray));
        if (dist <= RENDER_DIST && dist2 < RENDER_DIST) {
            return dist;
        }
        dist = get_distance(w2,position);
        dist2 = get_distance(w2,set_length(RENDER_DIST,ray));
        if (dist <= RENDER_DIST && dist2 < RENDER_DIST) {
            return dist;
        }
        return (float) -1;
    } else if (ray.x ==0) {
        float a_wall = (w1.y-w2.y) / (w1.x-w2.x);
        float b_wall = w1.y - a_wall * w1.x;
        hit_x = position.x;
        hit_y = a_wall * position.x + b_wall;
    } else {
        float a_wall = (w1.y-w2.y) / (w1.x-w2.x);
        float b_wall = w1.y - a_wall * w1.x;
        hit_x = (b_ray - b_wall) / (a_wall - a_ray);
        hit_y = a_wall * hit_x + b_wall;
    }
    float dist = get_distance((struct v2_f) {hit_x,hit_y}, position);
    float dist2 = get_distance((struct v2_f) {hit_x,hit_y},set_length(RENDER_DIST,ray));
    if (dist <= RENDER_DIST && dist2 < RENDER_DIST) {
        return dist;
    }
    return (float) -1;
}