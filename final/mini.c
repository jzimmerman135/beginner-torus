// here is the code
// compile (on unix and maybe puTTY): gcc mini.c -o mini -lncurses
// compile (on windows console):
// git clone https://github.com/wmcbrine/PDCurses.git
// cd wincon
// NOT SURE IF THESE WILL JUST WORK, kinda guessing here
// make
// mv pdcurses.o and ../curses.h to same directory as mini
// gcc mini.c pdcurses.o -o mini
#include <curses.h>
#include <time.h>
#include <math.h>
#define min(a, b) ((a) > (b) ? (b) : (a)) // math functions 
float clamp(float a, float b, float c) { return a < b ? b : a > c ? c : a; }
float lerp(float a, float b, float t) { return a+(b-a)*clamp(t,0.0,1.0); }
float hypotenuse(float x, float y) { return sqrtf(x * x + y * y); }
typedef struct { float x; float y; float z; } float3; // vector functions 
float length3(float3 v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }
float3 f3(float x, float y, float z) { return (float3){x, y, z}; }
float3 max3(float3 a, float3 b) {
    return f3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z)); 
}
float3 scale3(float3 a, float b) { return f3(a.x * b, a.y * b, a.z * b); }
float3 norm(float3 a) { return scale3(a, 1.0f / length3(a)); }
float3 cross3(float3 a, float3 b) {
    return f3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y * b.x);
}

#define MAX_TIME 20.0
#define RAY_ORIGIN 0, 6.0, lerp(-10.0, -6.0, t - 5.0) 
#define LIGHT_ORIGIN 10.0 * sinf(t / 3.0), 10, 2.0

char  colors[] = {'`', '@', '#', '#', '*', '+', '=', ':', '-', '.', ' '};
float levels[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

float torus_SDF(float3 p, float r1, float r2) { // distance field of torus
    return hypotenuse(hypotenuse(p.x, p.z) - r1, p.y) - r2;
}

float cube_SDF(float3 p, float r) { // distance field of cube
    float3 q = {fabsf(p.x) - r, fabsf(p.y) - r, fabsf(p.z) - r};
    float n = fminf(fmaxf(fmaxf(q.x, q.y), q.z), 0.0);
    return length3(max3(q, f3(0, 0, 0))) + n;
}

float dist(float3 p, float t) { // get distance
    float d_donut = torus_SDF(p, 2.0, 0.75), d_cube = cube_SDF(p, 2.0) - 0.5;
    return lerp(d_donut, d_cube, cosf(t) * 0.5 + 0.5);
}

char brightness_to_ascii(float b) { // brightness to ascii
    unsigned i = 0;
    while (b > levels[i] && i < sizeof(colors) / sizeof(*colors)) i++;
    return colors[i]; 
}

float render(float x, float y, float t) { // coordinates to brightness
    float3 p, ro = {RAY_ORIGIN}, lo = {LIGHT_ORIGIN}; 
    float3 f = norm(f3(-ro.x,-ro.y,-ro.z)), r = norm(cross3(f3(0,-1,0), f));
    float3 u = norm(cross3(f, r));
    float3 rd = {f.x+r.x*x+u.x*y,f.y+r.y*x+u.y*y,f.z+r.z*x+u.z*y};
    float d = 0.0, ds;
    for (int i = 0; i < 100; i++) {
        p = f3(ro.x + rd.x * d, ro.y + rd.y * d, ro.z + rd.z * d);
        ds = dist(p, t);
        d += ds;
        if (d > 15.0)
            return -1.0;
        if (fabsf(ds) < 0.01)
            break;
    }
    float3 dx = f3(p.x-0.01,p.y,p.z);
    float3 dy = f3(p.x,p.y-0.01,p.z);
    float3 dz = f3(p.x,p.y,p.z-0.01);
    float3 n = norm(f3(ds - dist(dx, t), ds - dist(dy, t), ds - dist(dz, t)));
    float rb = fmaxf((-n.y*.5+.5) * 0.5 - 0.05, 0.02);
    lo = norm(f3(lo.x-p.x, lo.y-p.y, lo.z-p.z));
    return clamp(lo.x*n.x+lo.y*n.y+lo.z*n.z, rb, 1.0);
}

void draw_scene(float w, float h, float t) {
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++) {
            float b = render((2 * i - w) / (4 * h), (j / h) - 0.5, t);
            char a = brightness_to_ascii(b);
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
