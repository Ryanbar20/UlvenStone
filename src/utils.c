
#define PI              3.14159265359
#define EPSILON         0.00001f
#define RAD_TO_DEG(x)   x * (180 / PI)
#define DEG_TO_RAD(x)   x * (PI / 180)

typedef struct v2_f{
    float x;
    float y;
} v2_f;

v2_f view;
v2_f pos;

//we skip the sqrt stage because its expensive and only needed if wall hit
static inline float get_distance_squared(v2_f v1, v2_f v2) {
    return (v1.x-v2.x)*(v1.x-v2.x)  +  (v1.y-v2.y)*(v1.y-v2.y);
}

static inline float get_length(v2_f vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

static inline v2_f add_vectors(v2_f v1, v2_f v2) {
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

    if (ray.x == 0) ray.x += EPSILON;
    const float a_ray     = ray.y / ray.x ;
    const float b_ray     = pos.y - a_ray * pos.x;
    // if wall is parralel, it wont hit
    const int v1_under    = w1.y < (w1.x * a_ray + b_ray);
    const int v2_under    = w2.y < (w2.x* a_ray + b_ray);
    if (v1_under == v2_under) return -1;
    
    float hit_x = w1.x;
    if (w1.x!=w2.x) {
        const float a_wall  = (w1.y-w2.y) / (w1.x-w2.x);
        const float b_wall  = w1.y - a_wall * w1.x;
        hit_x               = (b_ray - b_wall) / (a_wall - a_ray);
    }
    float hit_y             = a_ray * hit_x + b_ray;

    const float dist        = get_distance_squared((v2_f) {hit_x,hit_y}, pos);
    const v2_f sized_ray    = add_vectors(pos, set_length(RENDER_DIST,ray)); 
    const float dist2       = get_distance_squared((v2_f) {hit_x,hit_y},sized_ray);
    if (dist2 >= RENDER_DIST*RENDER_DIST ||dist > RENDER_DIST*RENDER_DIST ) return -1;
    return sqrt(dist);
}