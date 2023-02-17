#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include "vec.h"

/* brightness < 0 corresponds to the background using the '`' character */
char  colors[] = {'`', '@', '%', '#', '*', '+', '=', ':', '-', '.', ' '};
float levels[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

/* N_LEVELS adapts automatically if you add more levels */
const int N_LEVELS = sizeof(levels) / sizeof(*levels); 

/* program settings macros (change freely) */
#define MAX_TIME 5.0
#define MAX_WINDOW_W 300
#define MAX_WINDOW_H 120
#define WINDOW_STRETCH_FACTOR 2.0 

/* rendering performance macros */
#define MAX_DIST 10.0
#define MAX_STEPS 100
#define SURFACE_DIST 0.01

/* camera setting macros */
#define LOOKAT_PT 0.0, 0.0, 0.0
#define CAMERA_ORIGIN 0.0, 2.0, -6.0
#define LIGHT_ORIGIN 4.0, 10.0 + 5.0 * sin(4.0 * time), 0.0

/* helper functions */
float3 get_ray_direction(float2 uv, float3 p, float3 l, float z);
float sphere_SDF(float3 p, float r);

/* rendering functions */
float get_distance(float3 position, float time) {
    (void)time;
    position.y += sin(time) * 0.5 - 0.5;
    float distance = sphere_SDF(position, 2.0);
    return distance;
}

float raymarch(float3 origin, float3 direction, float time) {
    float dist_origin = 0.0;
    for (int i = 0; i < MAX_STEPS; i++) {
        float3 position = add3(origin, scale3(direction, dist_origin));
        float dist_scene = get_distance(position, time); 
        dist_origin += dist_scene;
        if (dist_origin >= MAX_DIST || fabsf(dist_scene) < SURFACE_DIST) {
            return dist_origin;
        }
    }
    return dist_origin;
}

float3 get_normal(float3 position, float time) {
    float3 p = position;
    float d = get_distance(position, time);
    float e = 0.01;
    float3 gradient = {
        d - get_distance(f3new(p.x - e, p.y, p.z), time),
        d - get_distance(f3new(p.x, p.y - e, p.z), time),
        d - get_distance(f3new(p.x, p.y, p.z - e), time),
    };
    return normalize3(gradient);
}

float get_light(float3 position, float3 normal, float3 light_origin) {
    float3 light_direction = subtract3(light_origin, position);
    float diffuse = dot3(light_direction, normal);
    float down_angle = -normal.y * 0.5 + 0.5;
    float reflected_brightness = fmax(down_angle * 0.4, 0.001);
    return clamp(diffuse, reflected_brightness, 1.0);
}

/* drawing functions */
float coordinate_to_brightness(float x, float y, float time) {
    float2 uv = f2new(x, y);
    float3 lookat_pt = f3new(LOOKAT_PT);
    float3 ray_origin = f3new(CAMERA_ORIGIN);
    float3 light_origin = f3new(LIGHT_ORIGIN);
    float3 ray_direction = get_ray_direction(uv, ray_origin, lookat_pt, 1.0); 
    float distance = raymarch(ray_origin, ray_direction, time);
    if (distance >= MAX_DIST) {
        return -1.0;
    }

    float3 position = add3(ray_origin, scale3(ray_direction, distance));
    float3 normal = get_normal(position, time);
    float brightness = get_light(position, normal, light_origin);
    return brightness;
}

char brightness_to_ascii(float brightness) {
    int i = 0;
    while (brightness > levels[i] && i < N_LEVELS)
        i++;
    return colors[i];
}

void draw_scene(int width, int height, float time) {
    float aspect_ratio = (float)width / ((float)height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            move(j, i);
            float x = 2.0f * (float)i / (float)width - 1.0f;
            float y = 2.0f * (float)j / (float)height - 1.0f;
            x *= aspect_ratio / WINDOW_STRETCH_FACTOR;
            float brightness = coordinate_to_brightness(x, y, time);
            float ascii_pixel = brightness_to_ascii(brightness);
            addch(ascii_pixel);
        }
    }
}

/* shape functions */

int main()
{
    initscr();
    curs_set(0);
    clock_t time_start = clock();
    float time_elapsed = 0.0;
    int frames = 0;
    do {
        time_elapsed = (float)(clock() - time_start) / CLOCKS_PER_SEC;
        draw_scene(COLS, LINES, time_elapsed);
        refresh();
        frames++;
    } while (time_elapsed < MAX_TIME);
    endwin();
    printf("Framerate %.0f fps\n", frames / time_elapsed);
    return 0;
}

/* RENDERING FUNCTIONS */
/* ------------------- */
float3 get_ray_direction(float2 uv, float3 p, float3 l, float z)
{
    (void)l;
    float3 f = normalize3(scale3(p, -1.0));
    float3 r = normalize3(cross3(f3new(0, -1, 0), f));
    float3 u = cross3(f, r);
    float3 c = scale3(f, z);
    r = scale3(r, uv.x / 2.0);
    u = scale3(u, uv.y / 2.0);
    float3 rd = {c.x + r.x + u.x, c.y + r.y + u.y, c.z + r.z + u.z};
    return normalize3(rd);
}

/* DRAWING FUNCTIONS */
/* ----------------- */

/* 3D SHAPE SIGNED DISTANCE FIELD FUNCTIONS */
/* ---------------------------------------- */
/*
 * p = current position
 * r = radius
 * returns distance to the sphere
 */
float sphere_SDF(float3 p, float r)
{
    return length3(p.x, p.y, p.z) - r;
}