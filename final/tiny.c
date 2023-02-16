#include <curses.h>
#include <time.h>
#include "othervec.h"

// settings
const float MAX_TIME = 20;
const int MAX_WINDOW_W = 300;
const int MAX_WINDOW_H = 120;
const int WINDOW_STRETCH_FACTOR = 2;

const int MAX_STEPS = 100;
const float MAX_DIST = 10.0;
const float SURF_DIST = 0.01;

const vec3 CAMERA_POSITION = vec3(0, 5, -5);
const vec3 LIGHT_POSITION = vec3(-5, 10, 2);
const vec3 LOOKAT_POINT = vec3(0, 0, 0);
const float ZOOM = 1.0;

char  pixel_color[] = {'`', '%', '#', '@', '*', '+', '=', ':', '-', '.', ' '};
float pixel_level[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
const int N_PIXEL_LEVELS = sizeof(pixel_color) / sizeof(*pixel_color); 

/*******************************************************************************
                     SHAPES AND SIGNED DISTANCE FUNCTIONS
*******************************************************************************/
float 
get_cube_distance(vec3 p, float r)
{
    vec3 q = {fabsf(p.x) - r, fabsf(p.y) - r, fabsf(p.z) - r};
    vec3 m = {fmaxf(q.x, 0.0), fmaxf(q.y, 0.0), fmaxf(q.z, 0.0)};
    float n = fminf(fmaxf(fmaxf(q.x, q.y), q.z), 0.0);
    return length3(m) + n;
}

float
get_torus_distance(vec3 p, float r1, float r2)
{
    return length2(vec2(sqrtf(p.x * p.x + p.z * p.z) - r1, p.y)) - r2;
}

float
get_distance(vec3 point, float time)
{
    (void)time;
    point = rotateX(point, time);
    point = rotateY(point, time * 3.);
    point = rotateZ(point, time);
    float distance_to_donut = get_torus_distance(point, 2.0, 0.75);
    float distance_to_cube = get_cube_distance(point, 1.25) - 0.25;
    
    float curve = (tanh(time - 10.0) - tanh(time-15.0)) * .5;
    float distance = lerp(distance_to_donut, distance_to_cube, curve);
    return max(distance, 0);
}

/*******************************************************************************
                          RAY TRACING FUNCTIONS
*******************************************************************************/
vec3
get_ray_direction(vec2 scr_coord, vec3 camera_pos, vec3 lookat_pos, float zoom)
{
    vec3 look_vector = normalize3(subtract3(lookat_pos, camera_pos));
    vec3 right_vector = cross3(vec3(0, 1, 0), look_vector);
    vec3 up_vector = cross3(look_vector, right_vector); 
    look_vector = scale3(look_vector, zoom); 
    right_vector = scale3(right_vector, scr_coord.x);
    up_vector = scale3(up_vector, scr_coord.y);
    vec3 ray_direction = add3(look_vector, add3(up_vector, right_vector));
    return normalize3(ray_direction);
} 

float
get_ray_length(vec3 ray_origin, vec3 ray_direction, float time)
{
    float distance_from_origin = 0.0; 
    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 ray_position = scale_and_add3(ray_origin,
            ray_direction, distance_from_origin);

        float distance_from_scene = get_distance(ray_position, time);
        distance_from_origin += distance_from_scene;

        if  (fabsf(distance_from_scene) < SURF_DIST) // hit an object
            return distance_from_origin;

        if (distance_from_origin > MAX_DIST) // hit background
            return -1.0;
    }

    return -1.0; // reached max steps
}

vec3 
get_normal_vector(vec3 hit_position, float time)
{
    float distance = get_distance(hit_position, time);
    float e = 0.01;
    vec3 normal = {
        distance - get_distance(subtract3(hit_position, vec3(e, 0, 0)), time),
        distance - get_distance(subtract3(hit_position, vec3(0, e, 0)), time),
        distance - get_distance(subtract3(hit_position, vec3(0, 0, e)), time)
    };
    return normalize3(normal);
} 

float
get_light(vec3 hit_position, vec3 light_position, float time)
{
    vec3 light_direction = normalize3(subtract3(light_position, hit_position));    
    vec3 hit_normal = get_normal_vector(hit_position, time);
    float diffuse_light = clamp(dot3(light_direction, hit_normal), 0.001, 1.0);
    return diffuse_light;
}

// drawing functions
float get_pixel_brightness(vec2 screen_coord, float time) 
{
    vec3 ray_origin = CAMERA_POSITION;
    vec3 lookat_point = LOOKAT_POINT;
    vec3 light_position = LIGHT_POSITION;
    float zoom = ZOOM; 

    vec3 ray_direction = get_ray_direction(screen_coord, ray_origin,
        lookat_point, zoom);

    float ray_length = get_ray_length(ray_origin, ray_direction, time);

    if (ray_length == -1.0) {
        return -1.0;
    }

    vec3 hit_position = scale_and_add3(ray_origin, ray_direction, ray_length);
    float brightness = get_light(hit_position, light_position, time);

    return brightness;
}

char brightness_to_ascii(float brightness)
{
    int i = 0;
    while (brightness > pixel_level[i] && i < N_PIXEL_LEVELS)
            i++;
    return pixel_color[i]; 
}

void draw_scene(int w, int h, float time)
{
    float aspect_ratio = (float)w / ((float)h * WINDOW_STRETCH_FACTOR); 
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            float screen_x = (float)i / (float)w * 2.0 - 1.0;
            float screen_y = (float)j / (float)h * 2.0 - 1.0;

            vec2 screen_coord = {screen_x * aspect_ratio, -screen_y};

            float pixel_brightness = get_pixel_brightness(screen_coord, time);
            char ascii_output = brightness_to_ascii(pixel_brightness);

            move(j, i); 
            addch(ascii_output);
        }
    }
}

int main()
{
    initscr();
    curs_set(0);

    clock_t time_start = clock();
    float seconds_elapsed;

    int w = min(MAX_WINDOW_W, COLS);
    int h = min(MAX_WINDOW_H, COLS);
    int frame_count = 0;

    do {
        seconds_elapsed = (float)(clock() - time_start) / CLOCKS_PER_SEC;
        draw_scene(w, h, seconds_elapsed);
        refresh();
        frame_count++;
    } while (seconds_elapsed < MAX_TIME);

    endwin();
    
    float framerate = (float)frame_count / seconds_elapsed;
    printf("Animation complete\nFramerate %.1f fps\n", framerate);

    return 0;
}
