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
#define MAX_TIME 5.0
#define MAX_WINDOW_W 300
#define MAX_WINDOW_H 120
#define WINDOW_STRETCH_FACTOR 2.0 

/* rendering performance macros */
/* @TODO */

/* camera setting macros */
/* @TODO */

/* rendering functions */
/* @TODO */

/* drawing functions */
float coordinate_to_brightness(float x, float y, float time) {
    float radius = sin(time) * 0.25 + 0.5;
    if (length2(x, y) < radius) {
        return 0.3;
    }
    return -1.0;
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