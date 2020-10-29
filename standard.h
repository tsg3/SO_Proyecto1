#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <unistd.h>

ALLEGRO_BITMAP *background;
ALLEGRO_BITMAP *rock;
ALLEGRO_EVENT event;

int steps;
int imageWidth;
int imageHeight;
int current_x;
int current_y;

typedef struct MARCIAN
{
    int id;
    float pos_x;
    float pos_y;
    int energy;
    int period;
    char direction;
} MARCIAN;

MARCIAN marcians[3];

char addresses[4] = {'u', 'd', 'l', 'r'};

char maze[17][17] = {
    {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'},
    {'o', 'f', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'f', 'o', 'f', 'f', 'f', 'f', 'o'},
    {'o', 'f', 'o', 'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'f', 'o', 'o', 'o', 'o'},
    {'o', 'f', 'o', 'o', 'f', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'f', 'o', 'o', 'o', 'o'},
    {'o', 'f', 'o', 'o', 'f', 'o', 'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'f', 'o'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'f', 'o', 'f', 'o', 'o', 'o', 'o', 'o', 'o', 'f', 'o'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'f', 'o', 'o', 'f', 'f', 'f', 'f', 'o', 'o', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'o', 'f', 'o', 'o', 'f', 'o', 'o', 'o', 'o', 'o', 'f', 'o'},
    {'o', 'f', 'o', 'o', 'o', 'o', 'f', 'o', 'o', 'f', 'o', 'o', 'o', 'o', 'o', 'f', 'o'},
    {'o', 'f', 'o', 'o', 'o', 'o', 'f', 'o', 'o', 'f', 'o', 'o', 'f', 'f', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'o', 'f', 'o', 'o', 'f', 'o', 'o', 'f', 'o'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'f', 'o', 'o', 'f', 'o', 'o', 'o', 'f', 'f', 'f', 'o'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'f', 'o', 'o', 'f', 'o', 'o', 'o', 'f', 'o', 'o', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'o', 'f', 'o', 'o', 'o', 'f', 'o', 'o', 'o'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'f', 'f', 'f', 'f', 'o', 'o', 'f', 'f', 'f', 'f', 'm'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'}};
    