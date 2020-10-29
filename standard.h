#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <unistd.h>

ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_DISPLAY *disp;
ALLEGRO_FONT *font;
ALLEGRO_BITMAP *background;
ALLEGRO_BITMAP *rock;
ALLEGRO_BITMAP *marcian_image;
ALLEGRO_EVENT event;

int lenght;
int steps;
int imageWidth;
int imageHeight;
int current_x;
int current_y;
int window_height;
int window_width;
char current_window;  // m: menu, g: gaming, r: report, a: editing automatic
char game_mode; // a: automatic, m: manual

// New marcians parameters
int new_energy;
int new_period;

// Text interface

const char title[] = "STUPID MARCIANS";
const char m_mode_guide[] = "Press S to Manual mode";
const char a_mode_guide[] = "Press A to Automatic mode";
const char a_mode[] = "AUTOMATIC MODE";
const char m_mode[] = "MANUAL MODE";
const char period_text[] = "Period:";
const char energy_text[] = "Energy:";
const char add_space[] = "Press SPACE to add new marcian";
const char start_space[] = "Press SPACE to start simulation";
const char add_a[] = "Press A to add new marcian";
const char add_marcian_text[] = "Add new marcian";
const char modify_values_text[] = "To change values press:";
const char up_down_text[] = "Up / Down keys";
const char r_l_text[] = "Right / Lefts Keys";
const char finish_text[] = "Press X to finish simulation";
const char exit_text[] = "Press X to exit";

float time_pause = 0.3;

// Running game variable
bool running;

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
