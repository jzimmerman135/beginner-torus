#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include "vec.h"
#include "utils.h"

#define MAX_TIME              20.0
#define MAX_WINDOW_W          140
#define MAX_WINDOW_H          60
#define WINDOW_STRETCH_FACTOR 2. 

#define MAX_STEPS 100
#define MAX_DIST  10.0
#define SURF_DIST 0.01

#define RAY_ORIGIN 0, 5, -5
#define LIGHT_ORIGIN -5.0, 10, 2.0
#define LOOKAT_POSITION 0, 0, 0
#define ZOOM 1.0

/* shapes */
float cube_SDF(float3 p, float r);
float torus_SDF(float3 p, float r1, float r2);

/* rendering functions */
float3 ray_dir(float2 uv, float3 p, float3 l, float z);
float raymarch(float3 ro, float3 rd, float t);
float get_distance(float3 p, float t);
float3 get_normal(float3 p, float t);
float get_light(float3 cp, float3 lo, float t);

/* drawing functions */
float coordinate_to_brightness(float x, float y, float t);
char brightness_to_ascii(float b);
void draw_scene(int w, int h, float t);

int main()
{ 
    printf("Hello Workshop\n");
    return 0;
}

float torus_SDF(float3 p, float r1, float r2)
{
    /* math magic -- source: inigo quilez */
    return length2f(length2f(p.x, p.z) - r1, p.y) - r2;
}

float cube_SDF(float3 p, float r)
{
    float3 q = {fabsf(p.x) - r, fabsf(p.y) - r, fabsf(p.z) - r};
    float3 m = {fmaxf(q.x, 0.0), fmaxf(q.y, 0.0), fmaxf(q.z, 0.0)};
    float n = fminf(fmaxf(fmaxf(q.x, q.y), q.z), 0.0);
    return length3f(m.x, m.y, m.z) + n;
}