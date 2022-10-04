#include "vec.h"
#include "math.h"

float clamp(float a, float b, float c)
{
    if (b < a)
        return b;
    else if (a > c)
        return c;
    return a;
}

/*******************************************************************************
                               VEC2 FUNCTIONS 
 ******************************************************************************/

struct vec2 vec2(float x, float y)
{
    struct vec2 v = {x, y};
    return v;
}

float length2(float x, float y)
{
    return sqrt(x * x + y * y);
}

/*******************************************************************************
                               VEC3 FUNCTIONS 
 ******************************************************************************/

struct vec3 vec3(float x, float y, float z)
{
    struct vec3 v = {x, y, z};
    return v;
}

float length3(float x, float y, float z)
{
    return sqrt(x * x + y * y + z * z);
}

struct vec3 normalize3(struct vec3 a)
{
    float l = length3(a.x, a.y, a.z);
    return scale3(a, 1.0 / l);
}

struct vec3 add3(struct vec3 a, struct vec3 b) 
{
    struct vec3 v = {a.x + b.x, a.y + b.y, a.z + b.z};
    return v; 
}

struct vec3 subtract3(struct vec3 a, struct vec3 b) 
{
    struct vec3 v = {a.x - b.x, a.y - b.y, a.z - b.z};
    return v; 
}

struct vec3 scale3(struct vec3 a, float s)
{
    struct vec3 v = {a.x * s, a.y * s, a.z * s};
    return v;
}

float dot3(struct vec3 a, struct vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
