#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <math.h>
#include "vec.h"

/* brightness < 0 corresponds to the background using the '`' character */
char  colors[] = {'`', '@', '%', '#', '*', '+', '=', ':', '-', '.', ' '};
char  dark_mode[] = {'_', ' ', '.', '-', ':', '=', '+', '*', '#', '%', '@'};
float levels[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

/* N_LEVELS adapts automatically if you add more levels */
const int N_LEVELS = sizeof(levels) / sizeof(*levels); 

/* program settings macros (change freely) */
#define MAX_TIME 20.0
#define MAX_WINDOW_W 300
#define MAX_WINDOW_H 120
#define WINDOW_STRETCH_FACTOR 2. 

/* rendering performance macros */
/* @TODO */

/* camera setting macros */
/* @TODO */

/* rendering functions */
/* @TODO */

/* drawing functions */
/* @TODO */

/* shape functions */
/* @TODO */

#define MY_MACRO 200

void printing_tutorial() {
    char chosen_character = colors[5];
    char c = 'R';
    printf("This prints a character: %c\n", chosen_character);
    printf("This prints an integer and a character: %i %c \n", MY_MACRO, c);
    printf("This prints a float to 3 decimal points: %.3f \n", 0.984223);

    // special debug
    fprintf(stderr, "This goes to debug.txt if you run \'make debug\' \n");
    fprintf(stderr, "This gets redirected too: %c %c\n", chosen_character, c);
}

int main()
{
    printing_tutorial();
    return 0;
}

/* RENDERING FUNCTIONS */
/* ------------------- */

/* DRAWING FUNCTIONS */
/* ----------------- */

/* 3D SHAPE SIGNED DISTANCE FIELD FUNCTIONS */
/* ---------------------------------------- */
