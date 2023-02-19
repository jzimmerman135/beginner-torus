#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include "vec.h"
#include "helpers.h"

/* brightness < 0 corresponds to the background using the '`' character */
char  colors[] = {'`', '@', '%', '#', '*', '+', '=', ':', '-', '.', ' '};
char  dark_mode[] = {'_', ' ', '.', '-', ':', '=', '+', '*', '#', '%', '@'};
float levels[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

/* N_LEVELS adapts automatically if you add more levels */
const int N_LEVELS = sizeof(levels) / sizeof(*levels); 

/* program settings macros (change freely) */
#define MAX_TIME 12.0
#define MAX_WINDOW_W 500
#define MAX_WINDOW_H 200
#define WINDOW_STRETCH_FACTOR 2.0 

/* rendering performance macros */
#define MAX_DIST 15.0
#define MAX_STEPS 80
#define SURFACE_DIST 0.01

/* camera setting macros */
#define LOOKAT_PT 0.0, 0.0, 0.0
#define CAMERA_ORIGIN 0.0, 2.0, -6.0
#define LIGHT_ORIGIN 4.0, 10.0 + 5.0 * sin(4.0 * time), 0.0

/* helper functions (located in helper.h) */
float3 get_ray_direction(float2 uv, float3 p, float3 l, float z);

// signed distance fields (SDF) return distance from p to the nearest point
// on the surface of the shape. Implicitly assumes that the shapes
// are positioned at (0, 0, 0). Returns a negative number if inside the shape.
float sphere_SDF(float3 p, float r);
float torus_SDF(float3 p, float r1, float r2);
float cube_SDF(float3 p, float r);

/* rendering functions */

/* THIS IS WHERE OUR SCENE IS DEFINED */
float get_distance(float3 position, float time) {
    // using transformations on position, we can lie to
    // each distance estimator about where we are in space,
    // this lets us design our scene
    float3 position_torus = position;
    position_torus.y -= sin(time) * 0.5 - 0.5;
    position_torus = rotate3Z(position_torus, time);
    position_torus = rotate3X(position_torus, 2.0 * time);

    float3 position_cube = add3(position, f3new(6, 0, -5));
    position_cube = add3(position_cube, f3new(cos(time), sin(time), 0));
    position_cube = rotate3Y(position_cube, time);
    position_cube = rotate3X(position_cube, 0.4);

    float torus_distance = torus_SDF(position_torus, 2.0, 0.75);

    float round_edges = 0.25;
    float cube = cube_SDF(position_cube, 1.0) - round_edges;
    float sphere = sphere_SDF(position_cube, 0.6);

    // interpolate between two shapes for a smooth transition
    float interpolation = (tanh(time - 4.0) * 0.5 + 0.5)
                            * (sin(3.0 * time) * 0.5 + 0.5);
    float other_shape_distance = lerp(cube, sphere, interpolation);

    float nearest_point = fminf(torus_distance, other_shape_distance);
    return nearest_point;
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
    float3 light_direction = normalize3(subtract3(light_origin, position));
    float diffuse = dot3(light_direction, normal);
    float down_angle = -normal.y * 0.5 + 0.5;
    float reflected_brightness = fmax(down_angle * 0.4, 0.001);
    return clamp(diffuse, reflected_brightness, 1.0);
}

/* drawing functions */
float coordinate_to_brightness(float x, float y, float time) {
    float2 xy = f2new(x, y);
    float3 lookat_pt = f3new(LOOKAT_PT);
    float3 ray_origin = f3new(CAMERA_ORIGIN);
    float3 light_origin = f3new(LIGHT_ORIGIN);
    float3 ray_direction = get_ray_direction(xy, ray_origin, lookat_pt, 1.0); 
    float distance_to_scene = raymarch(ray_origin, ray_direction, time);
    if (distance_to_scene >= MAX_DIST) {
        return -1.0;
    }

    float3 position = add3(ray_origin, scale3(ray_direction, distance_to_scene));
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
            y *= -1.0;
            float brightness = coordinate_to_brightness(x, y, time);
            char ascii_pixel = brightness_to_ascii(brightness);
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
        int width = min(COLS, MAX_WINDOW_W);
        int height = min(LINES, MAX_WINDOW_H);
        draw_scene(width, height, time_elapsed);
        refresh();
        frames++;
    } while (time_elapsed < MAX_TIME);
    endwin();
    printf("Framerate %.0f fps\n", frames / time_elapsed);
    return 0;
}
