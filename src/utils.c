
#define PI              3.14159265359
#define RAD_TO_DEG(x)   x * (PI / 180)
#define DEG_TO_RAD(x)   x * (180 / PI)



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
int check_hit(struct v2_f ray,struct v2_f position, struct v2_f w1, struct v2_f w2) {
    //hit if:
    //  1: wall.v1 is on the other side of the ray that wall.v2 is on
    //  2: the place that is hit is closer to pos than the length of the ray

    // 1:
    int v1_under;
    int v2_under;
    float a_ray = -1;
    float b_ray = -1;
    if (ray.x == 0) {
        v1_under = w1.x > position.x;
        v2_under = w2.x > position.x;

    } else {

        float a_ray = ray.y / ray.x ;
        float b_ray = position.y - a_ray * position.x;
        v1_under = w1.y < (w1.x * a_ray + b_ray);
        v2_under = w2.y < (w2.x* a_ray + b_ray);
    }
    if (v1_under == v2_under) {
        // both are on the same side of the ray -> no hit
        return -1;
    }
    

    // check if wall or ray is exactly vertical
    float hit_x;
    float hit_y;
    if (w1.x-w2.x ==0) {
        //.... 
        // close enough if either point of the wall is in render distance
        float dist = get_distance(w1,position);
        if (dist <= RENDER_DIST) {
            return dist;
        }
        dist = get_distance(w2,position);
        if (dist <= RENDER_DIST) {
            return dist;
        }
        return -1;
    } else if (ray.x ==0) {
        // get hit point by plugging pos.x in into the wall-line and then check dist
        float a_wall = (w1.y-w2.y) / (w1.x-w2.x);
        float b_wall = w1.y - a_wall * w1.x;
        hit_x = position.x;
        hit_y = a_wall * position.x + b_wall;
    } else {
        float a_wall = (w1.y-w2.y) / (w1.x-w2.x);
        float b_wall = w1.y - a_wall * w1.x;
        hit_x = (b_wall - b_ray) / (a_ray - a_wall);
        hit_y = a_wall * hit_x + b_wall;
    }
    float dist = get_distance((struct v2_f) {hit_x,hit_y}, position);
    if (dist <= RENDER_DIST) {
        return dist;
    }
    return -1;
}