#include "math.h"

/*******************************************************************************
                               FLOAT FUNCTIONS 
*******************************************************************************/

#ifndef min
#define min(a, b) fminf(a, b)
#endif 
#ifndef max
#define max(a, b) fmaxf(a, b)
#endif

float clamp(float a, float b, float c)
{
    return fmaxf(b, fminf(a, c));
}

float lerp(float a, float b, float t)
{
    return a + (b - a) * clamp(t, 0, 1);
}


/*******************************************************************************
                               VEC2 FUNCTIONS 
*******************************************************************************/

typedef struct vec2 {
    float x;
    float y;
} vec2;

// static inline
// vec2 vec2(float x, float y)
// {
//     return (vec2){x, y};
// }

#define vec2(x, y) ((vec2){(x), (y)})

static inline
float length2(vec2 a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}


/*******************************************************************************
                               VEC3 FUNCTIONS 
*******************************************************************************/

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

// static inline
// vec3 vec3(float x, float y, float z)
// {
//     return (vec3){x, y, z};
// }

#define vec3(x, y, z) ((vec3){(x), (y), (z)})

static inline
float length3(vec3 a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
} 

static inline
vec3 mult3(vec3 a, vec3 b)
{
    return (vec3){a.x * b.x, a.y * b.y, a.z * b.z};
}

static inline
vec3 normalize3(vec3 a)
{
    float length = length3(a);
    return (vec3){a.x / length, a.y / length, a.z / length};
}

static inline
vec3 add3(vec3 a, vec3 b)
{
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline
vec3 scale3(vec3 a, float b)
{
    return (vec3){a.x * b, a.y * b, a.z * b};
}

static inline
vec3 scale_and_add3(vec3 a, vec3 b, float c)
{
    return (vec3){a.x + b.x * c, a.y + b.y * c, a.z + b.z * c};
}

static inline
vec3 subtract3(vec3 a, vec3 b)
{
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline
float dot3(vec3 a, vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline
vec3 cross3(vec3 a, vec3 b)
{
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static inline
struct vec3 rotateX(vec3 a, float t)
{
    float ct = cos(t);
    float st = sin(t);
    return (vec3){
        a.x,
        a.z * ct + a.y * st,
        a.y * ct - a.z * st
    };
}

static inline
struct vec3 rotateY(vec3 a, float t)
{
    float ct = cos(t);
    float st = sin(t);
    return (vec3){
        a.x * ct - a.z * st,
        a.y,
        a.z * ct + a.x * st
    };
}

static inline
struct vec3 rotateZ(vec3 a, float t)
{
    float ct = cos(t);
    float st = sin(t);
    return (vec3){
        a.x * ct + a.y * st,
        a.y * ct - a.x * st,
        a.z
    };
}
