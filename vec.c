#include "vec.h"
#include "math.h"

float clamp(float a, float b, float c)
{
    return max(b, min(a, c));
}

float lerp(float a, float b, float t)
{
    return a + (b - a) * clamp(t, 0.0, 1.0);
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

struct vec3 cross3(struct vec3 a, struct vec3 b)
{
    struct vec3 v = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return v;
}

struct vec3 rotateY(float3 a, float t)
{
    struct vec3 v = {
        a.x * cos(t) - a.z * sin(t),
        a.y,
        a.z * cos(t) + a.x * sin(t)
    };
    return v;
}

struct vec3 rotateX(float3 a, float t)
{
    struct vec3 v = {
        a.x,
        a.z * cos(t) + a.y * sin(t),
        a.y * cos(t) - a.z * sin(t)
    };
    return v;
}

struct vec3 rotateZ(float3 a, float t)
{
    struct vec3 v = {
        a.x * cos(t) + a.y * sin(t),
        a.y * cos(t) - a.x * sin(t),
        a.z
    };
    return v;
}