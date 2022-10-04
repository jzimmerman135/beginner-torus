#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include "vec.h"

#define MAX_TIME 5.0
#define WINDOW_W 120
#define WINDOW_H 50
#define WINDOW_STRETCH_FACTOR 2. 

#define MAX_STEPS 200
#define MAX_DIST 50.0
#define SURF_DIST 0.001

/* rendering functions */
float raymarch(struct vec3 ro, struct vec3 rd);
float get_distance(struct vec3 p);
struct vec3 get_normal(struct vec3 p);
float get_light(struct vec3 cp, struct vec3 lo);
float torus_SDF(struct vec3 p, struct vec2 t);

/* drawing functions */
float coordinate_to_brightness(float x, float y, float t);
char brightness_to_ascii(float b);
void draw_scene(int w, int h, float t);

int main()
{ 
    initscr();      /* new blank screen */
    curs_set(0);    /* hide cursor      */

    clock_t time_start = clock();
    float time_elapsed;
    do { 

        time_elapsed = (float)(clock() - time_start) / CLOCKS_PER_SEC;

        draw_scene(WINDOW_W, WINDOW_H, time_elapsed);

        refresh();  /* output to screen */

    } while (time_elapsed < MAX_TIME);    

    endwin();       /* return to terminal */
    return 0;
}

/*
 * w = width in pixels
 * h = height in pixels
 * t = time in seconds (floating point value)
 */
void draw_scene(int w, int h, float t)
{
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            float x = (float)i / (float)w - 0.5;
            float y = (float)j / (float)h - 0.5;
            x *= (float)w / ((float)h * (float)WINDOW_STRETCH_FACTOR);

            float b = coordinate_to_brightness(x, y, t);
            char a = brightness_to_ascii(b);
            
            move(j, i);
            addch(a);
        }
    }
}

/*
 * b = brightness value
 */
char brightness_to_ascii(float b)
{
    char  colors[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    float levels[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    int n_levels = sizeof(colors) / sizeof(*colors); 
    
    if (b > levels[n_levels - 1])
        return colors[n_levels - 1];
    
    int i = 0;
    while (b > levels[i] && i < n_levels)
        i++;

    return colors[n_levels - i - 1]; 
}

/*
 * x = x coordinate in range [-0.5, 0.5]
 * y = y coordinate in range [-0.5, 0.5]
 * t = time
 */
float coordinate_to_brightness(float x, float y, float t)
{
    (void)t;
    struct vec3 ro = {0, 2.0, 0};
    struct vec3 rd = normalize3(vec3(x, y, 1));
    
    float d = raymarch(ro, rd);

    if (d > MAX_DIST) 
        return 0.0;

    struct vec3 p = add3(ro, scale3(rd, d));
    struct vec3 lo = {0, 10, sin(t) * 2}; 
    
    float dif = get_light(p, lo);
    return pow(dif, .25);
}

/*
 * ro = ray origin
 * rd = ray direction
 * returns distance from origin to scene along the ray direction 
 */
float raymarch(struct vec3 ro, struct vec3 rd)
{
    float d_origin = 0.0;
    
    for (int i = 0; i < MAX_STEPS; i++) {
        struct vec3 p = add3(ro, scale3(rd, d_origin)); 
        float d_scene = get_distance(p);
        d_origin += d_scene;

        if (d_origin > MAX_DIST || fabsf(d_scene) < SURF_DIST)
            break;
    }

    return d_origin;
}

/*
 * p = position in space
 * return the distance from the point in space to the scene
 */
float get_distance(struct vec3 p)
{
    // float d = torus_SDF(vec3(p.x, p.y - 1.0, p.z - 6.0), vec2(1., 1.));
    float r = 2.0;
    float d = length3(p.x, p.y - 1.5, p.z - 6.0) - r;
    return d;
}

/*
 * cp = contact surface position
 * return the normal vector of the surface at or very near position p
 */
struct vec3 get_normal(struct vec3 cp)
{
    float d = get_distance(cp);
    float e = 0.01;
    struct vec3 n = vec3(
        d - get_distance(vec3(cp.x - e, cp.y, cp.z)),
        d - get_distance(vec3(cp.x, cp.y - e, cp.z)),
        d - get_distance(vec3(cp.x, cp.y, cp.z - e))
    );

    return normalize3(n);
}

/*
 * cp = contact surface position
 * lo = light origin
 * ld = light direction
 * cn = contact surface normal
 * return the brightness of the position on the surface
 */
float get_light(struct vec3 cp, struct vec3 lo)
{
    struct vec3 ld = normalize3(subtract3(lo, cp));
    struct vec3 cn = get_normal(cp); 
    float dif = clamp(dot3(cn, ld), 0.01, 1.0);
    return dif;
}

/*
 * p = position in space
 * t = inner radius, outer radius
 * returns positive distance to the shape surface, negative if inside
 */
float torus_SDF(struct vec3 p, struct vec2 t)
{
    struct vec2 q = vec2(length2(p.x, p.z) - t.x, p.y);
    return length2(q.x, q.y) - t.y;
}
