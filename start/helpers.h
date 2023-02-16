#ifndef HELPER_INCLUDED
#define HELPER_INCLUDED
#include <vec.h>
#include <curses.h>
#include <time.h>
#include <math.h>

/* STARTER DECLARATIONS */
float3 ray_direction(float2 uv, float3 p, float3 l, float z);
float cube_SDF(float3 p, float r);
float torus_SDF(float3 p, float r1, float r2);
float sphere_SDF(float3 p, float r);

/* DRAWING FUNCTIONS */
/* ----------------- */

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

/* 3D SHAPE SIGNED DISTANCE FIELD FUNCTIONS */
/* ---------------------------------------- */
#ifdef SHAPE_HELPERS
/*
 * p = current position
 * r1 = outer radius
 * r2 = inner raidus
 * returns distance to the torus
 */
float torus_SDF(float3 p, float r1, float r2)
{  
    return length2(length2(p.x, p.z) - r1, p.y) - r2;
}

/*
 * p = current position
 * r = radius
 * returns distance to the cube
 */
float cube_SDF(float3 p, float r)
{
    float3 q = {fabsf(p.x) - r, fabsf(p.y) - r, fabsf(p.z) - r};
    float3 m = {fmaxf(q.x, 0.0), fmaxf(q.y, 0.0), fmaxf(q.z, 0.0)};
    float n = fminf(fmaxf(fmaxf(q.x, q.y), q.z), 0.0);
    return length3(m.x, m.y, m.z) + n;
}

/*
 * p = current position
 * r = radius
 * returns distance to the sphere
 */
float sphere_SDF(float3 p, float r)
{
    return length3(p.x, p.y, p.z) - r;
}
#endif

/* SAVE SOME TIME AND COPY PASTE THESE EASY FUNCTIONS */
extern const int N_LEVELS;
extern const float *levels;
extern const float *colors;

/*
 * b = brightness (darkest is 0.0 light is 1.0, background if negative)
 * returns the corresponding ascii character to draw
 */
char brightness_to_ascii(float b)
{
    int i = 0;
    while (b > levels[i] && i < N_LEVELS)
            i++;
    return colors[i]; 
}

/*
 *
 *
 */
#ifdef DRAW_HELPERS
void draw_scene(int w, int h, float t) {
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
#endif

#ifdef MAIN_HELPER
/*
 * copy and paste this stuff into main
 */
int main_copy() {
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
#endif

#endif