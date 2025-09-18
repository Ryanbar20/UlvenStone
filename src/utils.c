
#define PI              3.14159265359
#define RAD_TO_DEG(x)   x * (PI / 180)
#define DEG_TO_RAD(x)   x * (180 / PI)



struct v2_f {
    float x;
    float y;
};

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
int check_hit(struct v2_f ray,struct v2_f position, struct Wall *wall) {
    printf("%.2f %.2f %p",ray.x,ray.y, wall);

    //hit if:
    //  wall.v1 is on the other side of the ray that wall.v2 is on
    //  the place that is hit is closer to pos than the length of the ray

    return -1;
}