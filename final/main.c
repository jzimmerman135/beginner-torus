#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include "vec.h"

#define MAX_TIME 20.0
#define MAX_WINDOW_W 300
#define MAX_WINDOW_H 120
#define WINDOW_STRETCH_FACTOR 2. 

#define MAX_STEPS 100
#define MAX_DIST 10.0
#define SURF_DIST 0.01

#define RAY_ORIGIN 0, 5, -5
#define LIGHT_ORIGIN -5.0, 10, 2.0
#define LOOKAT_POSITION 0, 0, 0
#define ZOOM 1.0

char  colors[] = {'`', '%', '#', '@', '*', '+', '=', ':', '-', '.', ' '};
float levels[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
const int N_LEVELS = sizeof(colors) / sizeof(*colors); 

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
    initscr();      /* new blank screen */
    curs_set(0);    /* hide cursor      */

    clock_t time_start = clock();
    float time_elapsed; /* in seconds */

    int w = min(MAX_WINDOW_W, COLS);
    int h = min(MAX_WINDOW_H, LINES);
    int frames = 0;

    do { 
        time_elapsed = (float)(clock() - time_start) / CLOCKS_PER_SEC;     
        draw_scene(w, h, time_elapsed);
        refresh();  /* output to screen */
        frames++;
    } while (time_elapsed < MAX_TIME);    

    endwin();       /* return to command line */
    printf("Animation complete\nFramerate %.0f fps\n", frames / time_elapsed);
    return 0;
}

/*
* w = width in pixels
* h = height in pixels
* t = time in seconds (floating point value)
*/
void draw_scene(int w, int h, float t)
{
    float aspect_ratio = (float)w / ((float)h * WINDOW_STRETCH_FACTOR);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            /* i / w, j / h -> normalized to range (-0.5, 0.5) */
            float x = (float)i / (float)w - 0.5;
            float y = (float)j / (float)h - 0.5;

            /* correct for aspect ratio */
            x *= aspect_ratio;

            /* get scene brightness */
            float b = coordinate_to_brightness(x, y, t);

            /* get character to render at pixel */
            char a = brightness_to_ascii(b);
            
            /* draw to screen */
            move(j, i);
            addch(a);
        }
    }
}

/*
* b = brightness value
* returns corresponding character
*/
char brightness_to_ascii(float b)
{
    int i = 0;
    while (b > levels[i] && i < N_LEVELS)
            i++;
    return colors[i]; 
}

/*
 * x = x coordinate in range [-0.5, 0.5]
 * y = y coordinate in range [-0.5, 0.5]
 * t = time
 */
float coordinate_to_brightness(float x, float y, float t)
{
    float2 uv = vec2(x, y);
    float3 ro = {RAY_ORIGIN};
    float3 l = {LOOKAT_POSITION};
    float3 lo = {LIGHT_ORIGIN}; 
    float z = ZOOM;

    float3 rd = ray_dir(uv, ro, l, z); 
    
    float d = raymarch(ro, rd, t);

    if (d > MAX_DIST) { 
        return -1.0; /* background brightness */
    }

    float3 p = {ro.x + rd.x * d, ro.y + rd.y * d, ro.z + rd.z * d};
    float dif = get_light(p, lo, t);
    return dif;
}

/*
 * uv = pixel coordinate normalized
 * p = ray origin position
 * l = look at position
 * z = zoom
 */
float3 ray_dir(float2 uv, float3 p, float3 l, float z)
{
    uv.x = -uv.x;
    uv.y = -uv.y;
    float3 f = normalize3(subtract3(l, p));
    float3 r = normalize3(cross3(vec3(0, 1, 0), f));
    float3 u = cross3(f, r);
    float3 c = scale3(f, z);
    r = scale3(r, uv.x);
    u = scale3(u, uv.y);
    float3 rd = {c.x + r.x + u.x, c.y + r.y + u.y, c.z + r.z + u.z};
    return rd;
}

/*
 * ro = ray origin
 * rd = ray direction
 * t = time
 * returns distance from origin to scene along the ray direction 
 */
float raymarch(float3 ro, float3 rd, float t)
{
    float d_origin = 0.0;
    
    for (int i = 0; i < MAX_STEPS; i++) {
        float3 p = {
            ro.x + rd.x * d_origin, 
            ro.y + rd.y * d_origin,
            ro.z + rd.z * d_origin
        };

        float d_scene = get_distance(p, t);
        d_origin += d_scene;

        if (d_origin > MAX_DIST || fabsf(d_scene) < SURF_DIST)
            return d_origin;
    }

    return MAX_DIST + 1.;
}

/*
 * p = position in space
 * t = time
 * return the minimum distance from the point in space to the scene
 */
float get_distance(float3 p, float t)
{
    (void)t;
    p = rotateX(p, t);
    p = rotateY(p, t * 3.);
    p = rotateZ(p, t);
    float d_donut = torus_SDF(p, 2.0, 0.75);
    float d_cube = cube_SDF(p, 1.25) - 0.25;
    float d = lerp(d_donut, d_cube, (tanh(t - 10.0) - tanh(t-15.0)) * .5);
    return d;
}

/*
 * cp = contact surface position
 * return the normal vector of the surface at or very near position p
 */
float3 get_normal(float3 cp, float t)
{
    float d = get_distance(cp, t);
    
    /* using calculus approximation */
    float e = 0.01;
    float3 n = {
        d - get_distance(vec3(cp.x - e, cp.y, cp.z), t),
        d - get_distance(vec3(cp.x, cp.y - e, cp.z), t),
        d - get_distance(vec3(cp.x, cp.y, cp.z - e), t)
    };

    return normalize3(n);
}

/*
 * cp = contact surface position
 * lo = light origin
 * ld = light direction
 * cn = contact surface normal
 * return the brightness of the position on the surface
 */
float get_light(float3 cp, float3 lo, float t)
{
    /* get direction from light source to contact point */
    float3 ld = {lo.x - cp.x, lo.y - cp.y, lo.z - cp.z};
    ld = normalize3(ld);
    /* get normal of surface */ 
    float3 cn = get_normal(cp, t); 
    
    /* use to simulate reflections from floor */
    // float down_angle = -cn.y * .5 + .5; 
    // float reflected_brightness = fmax(down_angle * 0.4, 0.02); 

    /* measure closeness between surface normal and light source */
    float dif = clamp(dot3(ld, cn), 0.0001, 1.0);
    return dif;
}

/*
 * p = position in space
 * r1 = outer radius
 * r2 = inner radius
 * t = time
 * returns positive distance to the shape surface, negative if inside
 */
float torus_SDF(float3 p, float r1, float r2)
{
    /* math magic -- source: inigo quilez */
    return length2(length2(p.x, p.z) - r1, p.y) - r2;
}

float cube_SDF(float3 p, float r)
{
    float3 q = {fabsf(p.x) - r, fabsf(p.y) - r, fabsf(p.z) - r};
    float3 m = {fmaxf(q.x, 0.0), fmaxf(q.y, 0.0), fmaxf(q.z, 0.0)};
    float n = fminf(fmaxf(fmaxf(q.x, q.y), q.z), 0.0);
    return length3(m.x, m.y, m.z) + n;
}
