#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include "vec.h"

char  colors[] = {'`', '@', '%', '#', '*', '+', '=', ':', '-', '.', ' '};
float levels[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
const int N_LEVELS = sizeof(levels) / sizeof(*levels);

/* program settings macros */

/* rendering performance macros */

/* camera setting macros */

/* rendering functions */
float3 ray_direction(float2 uv, float3 p, float3 l, float z);

/* drawing functions */

/* shape functions */
float cube_SDF(float3 p, float r);
float torus_SDF(float3 p, float r1, float r2);
float sphere_SDF(float3 p, float r);

#define MY_MACRO 200

int main()
{
    char chosen_character = colors[5];
    printf("This prints a character: %c\n", chosen_character);
    printf("So does this: %c\n", MY_MACRO);
    fprintf(stderr, "This line goes to debug.txt if you run \'make debug\'\n");
    return 0;
}

/* RENDERING FUNCTIONS                                                        */
/* -------------------                                                        */

/*
 * uv = pixel coordinate normalized (-0.5, 0.5)
 * p = ray origin position
 * l = ray look at position
 * z = zoom
 * returns the normalized ray direction
 */
float3 ray_direction(float2 uv, float3 p, float3 l, float z)
{
    float3 f = normalize3(subtract3(l, p));
    float3 r = normalize3(cross3(vec3(0, 1, 0), f));
    float3 u = cross3(f, r);
    float3 c = scale3(f, z);
    r = scale3(r, uv.x);
    u = scale3(u, uv.y);
    float3 rd = {c.x + r.x + u.x, c.y + r.y + u.y, c.z + r.z + u.z};
    return rd;
}

/* DRAWING FUNCTIONS                                                          */
/* -----------------                                                          */

/* 3D SHAPE SIGNED DISTANCE FIELD FUNCTIONS                                   */
/* ----------------------------------------                                   */
/*
 * p = current position
 * r1 = outer radius
 * r2 = inner raidus
 */
float torus_SDF(float3 p, float r1, float r2)
{  
    return length2f(length2f(p.x, p.z) - r1, p.y) - r2;
}

/*
 * p = current position
 * r = radius
 */
float cube_SDF(float3 p, float r)
{
    float3 q = {fabsf(p.x) - r, fabsf(p.y) - r, fabsf(p.z) - r};
    float3 m = {fmaxf(q.x, 0.0), fmaxf(q.y, 0.0), fmaxf(q.z, 0.0)};
    float n = fminf(fmaxf(fmaxf(q.x, q.y), q.z), 0.0);
    return length3f(m.x, m.y, m.z) + n;
}

/*
 * p = current position
 * r = radius
 */
float sphere_SDF(float3 p, float r)
{
    return length3f(p.x, p.y, p.z) - r;
}
