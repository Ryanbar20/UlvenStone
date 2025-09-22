
#define PI              3.14159265359
#define RAD_TO_DEG(x)   x * (180 / PI)
#define DEG_TO_RAD(x)   x * (PI / 180)

typedef struct v2_f{
    float x;
    float y;
} v2_f;

v2_f view;
v2_f pos;

static inline float get_distance(v2_f v1, v2_f v2) {
    return sqrt((v1.x-v2.x)*(v1.x-v2.x)  +  (v1.y-v2.y)*(v1.y-v2.y));
}

static inline float get_length(v2_f vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

v2_f add_vectors(v2_f v1, v2_f v2) {
    return (v2_f) {v1.x+v2.x,v1.y+v2.y};
}

v2_f set_length(float len, v2_f vector) {
    float scale = len / get_length(vector);
    return (v2_f) {scale * vector.x, scale * vector.y};
}


static inline v2_f rotate(float rad, v2_f vector) {
    return (v2_f) {vector.x * cos(rad) - vector.y*sin(rad), vector.x * sin(rad) + vector.y*cos(rad)};
}

// returns -1 if not hit, else returns distance to hit
float check_hit(v2_f ray, v2_f w1, v2_f w2) {

    int v1_under;
    int v2_under;
    float a_ray = -1;
    float b_ray = -1;
    if (ray.x == 0) {
        v1_under    = w1.x > pos.x;
        v2_under    = w2.x > pos.x;
    } else {
        a_ray       = ray.y / ray.x ;
        b_ray       = pos.y - a_ray * pos.x;
        v1_under    = w1.y < (w1.x * a_ray + b_ray);
        v2_under    = w2.y < (w2.x* a_ray + b_ray);
    }
    if (v1_under == v2_under) { // if wall is parralel, it wont hit
        return -1.0f;
    }
    
    float hit_x;
    float hit_y;

    if (w1.x==w2.x) {
        
        hit_x               = w1.x;
        hit_y               = a_ray * hit_x + b_ray;
    } else {
        const float a_wall  = (w1.y-w2.y) / (w1.x-w2.x);
        const float b_wall  = w1.y - a_wall * w1.x;
        hit_x               = ray.x==0 ? pos.x : (b_ray - b_wall) / (a_wall - a_ray);
        hit_y               = a_wall * hit_x + b_wall;
    }

    const float dist        = get_distance((v2_f) {hit_x,hit_y}, pos);
    const v2_f sized_ray    = add_vectors(pos, set_length(RENDER_DIST,ray)); 
    const float dist2       = get_distance((v2_f) {hit_x,hit_y},sized_ray);
    if (dist <= RENDER_DIST && dist2 < RENDER_DIST) return dist;
    return -1.0f;
}