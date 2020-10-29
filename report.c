#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Cycles {
    int Id_Proc;
    int Exec_Cycles;
    struct Cycles* Next_Cycle;
} node_c;
typedef struct Colors {
    int Id_Proc;
    int R;
    int G;
    int B;
    struct Colors* Next_Color;
} color;
node_c* Cycles_Head = NULL;
color* Color_Head = NULL;
int Cycles_Count = 0;

int generate_random() {
    return rand() % 256; 
}

void add_color(int id) {
    if (id != -1) {
        if (Color_Head == NULL) {
            Color_Head = (color*)malloc(sizeof(color));
            Color_Head->Id_Proc = id;
            Color_Head->R = generate_random(); 
            Color_Head->G = generate_random(); 
            Color_Head->B = generate_random(); 
            Color_Head->Next_Color = NULL;
        }
        else {
            color* temp = Color_Head;
            while (temp->Next_Color != NULL) {
                if (temp->Id_Proc == id) {
                    return;
                }
                temp = temp->Next_Color;
            }
            if (temp->Id_Proc == id) {
                return;
            }
            color* new_node = (color*)malloc(sizeof(color));
            new_node->Id_Proc = id;
            new_node->R = generate_random(); 
            new_node->G = generate_random(); 
            new_node->B = generate_random(); 
            new_node->Next_Color = NULL;
            temp->Next_Color = new_node;
        }
    }
}

void add_executed_cycle(int id, int cycles) {
    Cycles_Count += cycles;
    if (Cycles_Head == NULL) {
        Cycles_Head = (node_c*)malloc(sizeof(node_c));
        Cycles_Head->Id_Proc = id;
        Cycles_Head->Exec_Cycles = cycles;
        Cycles_Head->Next_Cycle = NULL;
    }
    else {
        node_c* temp = Cycles_Head;
        while (temp->Next_Cycle != NULL) {
            temp = temp->Next_Cycle;
        }
        node_c* new_node = (node_c*)malloc(sizeof(node_c));
        new_node->Id_Proc = id;
        new_node->Exec_Cycles = cycles;
        new_node->Next_Cycle = NULL;
        temp->Next_Cycle = new_node;
    }
    add_color(id);
}

ALLEGRO_FONT* font;
int proc_len = 3;
int mode = 0;
int slider = 0;
int slider_state = 0;
int lines;
float scalar = 1.0;
int scalar_state = 0;
float total_length;

void draw_window() {
    al_clear_to_color(al_map_rgb(255, 255, 255));
    total_length = ((float) lines - 1.0) * 60 * scalar;
    if (slider_state == 1 && total_length - slider > 500) {
        slider += 2;
    }
    else if (slider_state == -1 && slider > 1) {
        slider -= 2;
    }
    
    if (scalar_state == 1 && scalar < 10.0) {
        scalar += 0.1;
    }
    else if (scalar_state == -1 && scalar > 1.1) {
        scalar -= 0.1;
    }

    char temp_title[25];
    char str[10];
    int i = 0;
    while(i < lines) {
        sprintf(str, "%d", i * 10);
        al_draw_line(120 + i * 60 * scalar - slider, 30, 
            120 + i * 60 * scalar - slider, 150, al_map_rgb(0, 0, 0), 1);
        al_draw_text(font, al_map_rgb(0, 0, 0), 
            120 + i * 60 * scalar - slider, 150, 0, str);
        i++;
    }

    node_c* temp = Cycles_Head;
    color* color_temp = Color_Head;
    int previous_x = 0;
    while (temp != NULL) {
        if (temp->Id_Proc != -1) {
            color_temp = Color_Head;
            while (color_temp != NULL) {
                if (color_temp->Id_Proc == temp->Id_Proc) {
                    break;
                }
                color_temp = color_temp->Next_Color;
            }
            al_draw_filled_rectangle(120 - slider + previous_x * scalar, 125, 
                120 - slider + (previous_x + temp->Exec_Cycles * 6) * scalar, 145, 
                al_map_rgb(color_temp->R, color_temp->G, color_temp->B));
            al_draw_rectangle(120 - slider + previous_x * scalar, 125, 
                120 - slider + (previous_x + temp->Exec_Cycles * 6) * scalar, 145, 
                al_map_rgb(0, 0, 0), 1);
        }
        previous_x += temp->Exec_Cycles * 6;
        temp = temp->Next_Cycle;
    }

    al_draw_text(font, al_map_rgb(0, 0, 0), 320, 210, 0, "Time");

    al_draw_filled_rectangle(0, 0, 119, 210, al_map_rgb(255, 255, 255));

    i = 0;
    while(i < proc_len) {
        strcpy(temp_title, "Marciano \0");
        sprintf(str, "%d", i + 1);
        strcat(temp_title, str);
        al_draw_text(font, al_map_rgb(0, 0, 0), 30, 45 + i * 30, 0, temp_title);
        i++;
    }

    if (mode == 0) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 30, 45 + i * 30, 0, "RMS");
    }
    else {
        al_draw_text(font, al_map_rgb(0, 0, 0), 30, 45 + i * 30, 0, "EDF");
    }

    al_flip_display();
}

void show_report() {
    /* Eliminar */
    srand(time(NULL));
    int cycles_ids[] = {0, 1, 2, 0, 2, 1, 2, 0, -1, -1, -1, -1, -1, 0, 1};
    int cycles_num[] = {1, 2, 3, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2};
    for (int i = 0; i < 15; i++) {
        add_executed_cycle(cycles_ids[i], cycles_num[i]);
    }
    lines = Cycles_Count;
    while (lines % 10 != 0) {
        lines++;
    }
    lines = lines / 10 + 1;

    /*          */

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);

    font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while(1) {

        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            redraw = true;
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    slider_state = -1;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    slider_state = 1;
                    break;
                case ALLEGRO_KEY_UP:
                    scalar_state = 1;
                    break;
                case ALLEGRO_KEY_DOWN:
                    scalar_state = -1;
                    break;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            redraw = true;
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    slider_state = 0;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    slider_state = 0;
                    break;
                case ALLEGRO_KEY_UP:
                    scalar_state = 0;
                    break;
                case ALLEGRO_KEY_DOWN:
                    scalar_state = 0;
                    break;
            }
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            draw_window();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}
