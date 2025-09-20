
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

struct v2_f add_vectors(struct v2_f v1, struct v2_f v2) {
    return (struct v2_f) {v1.x+v2.x,v1.y+v2.y};
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

    if (ray.x == 0) {
        if (w1.x-w2.x ==0) {
            if (ray.x != w1.x) {
                return (float) -1;
            }
            float dist = get_distance(w1,position);
            struct v2_f forward_check_point = add_vectors(position, set_length(RENDER_DIST,ray));
            // dist2 is to check that the wall is in front of the player.
            float dist2 = get_distance(w1,forward_check_point);
            if (dist <= RENDER_DIST && dist2 < RENDER_DIST) {
                return dist;
            }
            dist = get_distance(w2,position);
            dist2 = get_distance(w2,forward_check_point);
            if (dist <= RENDER_DIST && dist2 < RENDER_DIST) {
                return dist;
            }
            return (float) -1;
        } else {
        float a_wall = (w1.y-w2.y) / (w1.x-w2.x);
        float b_wall = w1.y - a_wall * w1.x;
        hit_x = position.x;
        hit_y = a_wall * hit_x + b_wall;
        }
    } else {
        if (w1.x-w2.x == 0) {
            hit_x = w1.x;
            hit_y = a_ray * hit_x + b_ray;


        } else {
            float a_wall = (w1.y-w2.y) / (w1.x-w2.x);
            float b_wall = w1.y - a_wall * w1.x;
            hit_x = (b_ray - b_wall) / (a_wall - a_ray);
            hit_y = a_wall * hit_x + b_wall;
        }

    }

    float dist = get_distance((struct v2_f) {hit_x,hit_y}, position);
    struct v2_f sized_ray = set_length(RENDER_DIST,ray);
    sized_ray.x = sized_ray.x + position.x;
    sized_ray.y = sized_ray.y + position.y;
    float dist2 = get_distance((struct v2_f) {hit_x,hit_y},sized_ray);
    if (dist <= RENDER_DIST && dist2 < RENDER_DIST) {
        return dist;
    }
    return (float) -1;
}