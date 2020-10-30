#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_DISPLAY *disp;
ALLEGRO_FONT *font;
ALLEGRO_BITMAP *background;
ALLEGRO_BITMAP *rock;
ALLEGRO_BITMAP *marcian_image;
ALLEGRO_EVENT event;

int length;
int steps;
int imageWidth;
int imageHeight;
int current_x;
int current_y;
int window_height;
int window_width;
char current_window; // m: menu, g: gaming, r: report, a: editing automatic
char game_mode;      // a: automatic, m: manual

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
const char r_l_text[] = "Right / Left Keys";
const char finish_text[] = "Press X to finish simulation";
const char exit_text[] = "Press X to exit";
const char mode_text_guide[] = "Scheduling mode:";

int time_pause = 500000;

// Running game variable
bool running;
bool redraw;

typedef struct Process
{
    int Id;
    int Max_Energy;
    int Current_Energy;
    int Period;
    struct Process *Next_Process;
    int *Offset;
    int State;
    float pos_x;
    float pos_y;
    char direction;
} node_p;

// MARCIAN marcians[10];

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

// Planner

int global_cycle;
int mode;
bool keep_execution = true;
pthread_t *planner;
int finished = 0;
int turn = -1;
int current_cycle = 0;
int multiple;

// Martians

node_p *head;
int *Offsets;
int Offsets_len;
pthread_t *threads;
int executed = 0;
int cycles = 0;

// Locks and conditions

pthread_cond_t cond_turn = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_exec = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock_exec = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_turn = PTHREAD_MUTEX_INITIALIZER;

// Report

typedef struct Cycles
{
    int Id_Proc;
    int Exec_Cycles;
    struct Cycles *Next_Cycle;
} node_c;

typedef struct ProcData
{
    int Id_Proc;
    int R;
    int G;
    int B;
    int Start;
    int Time;
    int End;
    int Period;
    struct ProcData *Next_Data;
} proc_data;

node_c *Cycles_Head;
proc_data *Data_Head = NULL;
int Cycles_Count;

ALLEGRO_FONT *font_2;
int slider;
int slider_state;
int lines;
float scalar;
int scalar_state;