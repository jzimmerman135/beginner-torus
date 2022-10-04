#ifndef VEC_INCLUDED
#define VEC_INCLUDED

float clamp(float a, float b, float c);

struct vec2 {
    float x;
    float y;
};

struct vec2 vec2(float x, float y);
float length2(float x, float y);

struct vec3 {
    float x;
    float y;
    float z;
};

struct vec3 vec3(float x, float y, float z);
float length3(float x, float y, float z);
struct vec3 normalize3(struct vec3 a);
struct vec3 scale3(struct vec3 a, float s);
struct vec3 add3(struct vec3 a, struct vec3 b);
struct vec3 subtract3(struct vec3 a, struct vec3 b);
float dot3(struct vec3 a, struct vec3 b);

#endif
