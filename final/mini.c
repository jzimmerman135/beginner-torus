#include <curses.h>
#include <time.h>
#include <math.h>

#define min(a, b) ((a) > (b) ? (b) : (a))
#define clamp(a, b, c) ((a) < (b) ? (b): (a) > (c) ? (c) : (a))
#define length2(x, y) (sqrtf((x) * (x) + (y) * (y)))
#define length3(x, y, z) (sqrtf((x) * (x) + (y) * (y) + (z) * (z)))
#define lerp(a, b, t) ((a) + ((b) - (a)) * clamp((float)(t), 0.0, 1.0))

#define MAX_TIME 20.0
#define RAY_ORIGIN 0, 6.0, lerp(-10.0, -6.0, t - 5.0) 
#define LIGHT_ORIGIN 10.0 * sinf(t / 3.0), 10, 2.0

typedef struct vec3 {
    float x;
    float y;
    float z;
} float3;

typedef struct vec2 {
    float x;
    float y;
} float2;

float3 vec3(float x, float y, float z) { // float3 constructor
    float3 v = {x, y, z};
    return v;
}

float3 nrm(float3 a) { // unit vector
    float l = length3(a.x, a.y, a.z);
    return vec3(a.x / l, a.y / l, a.z / l);
}

float3 cross(float3 a, float3 b) { // cross product
    return vec3(
        a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x
    );
}

char  colors[] = {'`', '@', '#', '#', '*', '+', '=', ':', '-', '.', ' '};
float levels[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

float torus_SDF(float3 p, float r1, float r2) { // distance field of torus
    return length2(length2(p.x, p.z) - r1, p.y) - r2;
}

float cube_SDF(float3 p, float r) { // distance field of cube
    float3 q = {fabsf(p.x) - r, fabsf(p.y) - r, fabsf(p.z) - r};
    float n = fminf(fmaxf(fmaxf(q.x, q.y), q.z), 0.0);
    return length3(fmaxf(q.x, 0.0),fmaxf(q.y, 0.0), fmaxf(q.z, 0.0)) + n;
}

float dist(float3 p, float t) { // get distance
    float d_donut = torus_SDF(p, 2.0, 0.75), d_cube = cube_SDF(p, 2.0) - 0.5;
    return lerp(d_donut, d_cube, cosf(t) * 0.5 + 0.5);
}

char pixelize(float b) { // brightness to ascii
    unsigned i = 0;
    while (b > levels[i] && i < sizeof(colors) / sizeof(*colors))
        i++;
    return colors[i]; 
}

float render(float x, float y, float t) { // coordinates to brightness
    float3 p, ro = {RAY_ORIGIN}, lo = {LIGHT_ORIGIN}; 
    float3 f = nrm(vec3(-ro.x, -ro.y, -ro.z));
    float3 r = nrm(cross(vec3(0, -1, 0), f));
    float3 u = nrm(cross(f, r));
    float3 rd = {f.x+r.x*x+u.x*y,f.y+r.y*x+u.y*y,f.z+r.z*x+u.z*y};
    float d = 0.0, ds;
    for (int i = 0; i < 100; i++) {
        p = vec3(ro.x + rd.x * d, ro.y + rd.y * d, ro.z + rd.z * d);
        ds = dist(p, t);
        d += ds;
        if (d > 15.0)
            return -1.0;
        if (fabsf(ds) < 0.01)
            break;
    }
    float3 dx = vec3(p.x-0.01,p.y,p.z);
    float3 dy = vec3(p.x,p.y-0.01,p.z);
    float3 dz = vec3(p.x,p.y,p.z-0.01);
    float3 n = nrm(vec3(ds - dist(dx, t), ds - dist(dy, t), ds - dist(dz, t)));
    float rb = fmaxf((-n.y * .5 + .5) * 0.5 - 0.05, 0.02);
    lo = nrm(vec3(lo.x-p.x, lo.y-p.y, lo.z-p.z));
    return clamp(lo.x * n.x + lo.y * n.y + lo.z * n.z, rb, 1.0);
}

void draw_scene(float w, float h, float t) {
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++) {
            float b = render((2 * i - w) / (4 * h), (j / h) - 0.5, t);
            char a = pixelize(b);
            move(j, i);
            addch(a);
        }
}

int main() { 
    initscr();
    curs_set(0);
    clock_t time_start = clock();
    float time_elapsed = 0.0;
    int frames = 0;
    do {
        time_elapsed = (float)(clock() - time_start) / CLOCKS_PER_SEC;
        draw_scene(min(300, COLS), min(120, LINES), time_elapsed);
        refresh();
        frames++;
    } while (time_elapsed < MAX_TIME);
    endwin();
    printf("Animation complete\nFramerate %.0f fps\n", frames / time_elapsed);
    return 0;
}
