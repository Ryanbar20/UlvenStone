
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