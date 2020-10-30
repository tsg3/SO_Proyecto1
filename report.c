#include "standard.h"

typedef struct Cycles {
    int Id_Proc;
    int Exec_Cycles;
    struct Cycles* Next_Cycle;
} node_c;
typedef struct ProcData {
    int Id_Proc;
    int R;
    int G;
    int B;
    int Start;
    int Time;
    int End;
    int Period;
    struct ProcData* Next_Data;
} proc_data;
node_c* Cycles_Head = NULL;
proc_data* Data_Head = NULL;
int Cycles_Count = 0;

int generate_random() {
    return rand() % 256; 
}

void add_data(int id, int start, int time, int period) {
    if (id != -1) {
        if (Data_Head == NULL) {
            Data_Head = (proc_data*)malloc(sizeof(proc_data));
            Data_Head->Id_Proc = id;
            Data_Head->R = generate_random(); 
            Data_Head->G = generate_random(); 
            Data_Head->B = generate_random(); 
            Data_Head->Start = start;
            Data_Head->Time = time;
            Data_Head->Period = period;
            Data_Head->End = 0;
            Data_Head->Next_Data = NULL;
        }
        else {
            proc_data* temp = Data_Head;
            while (temp->Next_Data != NULL) {
                if (temp->Id_Proc == id) {
                    return;
                }
                temp = temp->Next_Data;
            }
            if (temp->Id_Proc == id) {
                return;
            }
            proc_data* new_node = (proc_data*)malloc(sizeof(proc_data));
            new_node->Id_Proc = id;
            new_node->R = generate_random(); 
            new_node->G = generate_random(); 
            new_node->B = generate_random(); 
            new_node->Start = start;
            new_node->Time = time;
            new_node->Period = period;
            new_node->End = 0;
            new_node->Next_Data = NULL;
            temp->Next_Data = new_node;
        }
    }
}

void set_end (int id, int end) {
    proc_data* temp_data = Data_Head;
    while (temp_data->Id_Proc != id) {
        temp_data = temp_data->Next_Data;
    }
    temp_data->End = end;
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
}

ALLEGRO_FONT* font;
int mode_r;
int slider = 0;
int slider_state = 0;
int lines;
float scalar = 1.0;
int scalar_state = 0;
float total_length;

void draw_window() {
    al_clear_to_color(al_map_rgb(255, 255, 255));
    total_length = ((float) lines - 1.0) * 60 * scalar;
    if (slider_state == 1 && total_length - slider > 480) {
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
        sprintf(str, "%d", i);
        al_draw_line(120 + i * 60 * scalar - slider, 30, 
            120 + i * 60 * scalar - slider, 60 + length * 30, al_map_rgb(0, 0, 0), 1);
        al_draw_text(font, al_map_rgb(0, 0, 0), 
            120 + i * 60 * scalar - slider, 60 + length * 30, 0, str);
        i++;
    }

    proc_data* data_temp = Data_Head;
    int x = 0;
    int length_window;
    while (data_temp != NULL) {
        x = data_temp->Start * 6;
        length_window = data_temp->End * 6;
        while (x < length_window) {
            al_draw_filled_rectangle(120 - slider + x * scalar, 
                35 + (data_temp->Id_Proc) * 30, 
                120 - slider + (x + data_temp->Time * 6) * scalar, 
                55 + (data_temp->Id_Proc) * 30, 
                al_map_rgb(data_temp->R, data_temp->G, data_temp->B));
            al_draw_rectangle(120 - slider + x * scalar, 
                35 + (data_temp->Id_Proc) * 30, 
                120 - slider + (x + data_temp->Time * 6) * scalar, 
                55 + (data_temp->Id_Proc) * 30, 
                al_map_rgb(0, 0, 0), 1);
            x += data_temp->Period * 6;
        }
        data_temp = data_temp->Next_Data;
    }

    node_c* temp = Cycles_Head;
    int previous_x = 0;
    while (temp != NULL) {
        if (temp->Id_Proc != -1) {
            data_temp = Data_Head;
            while (data_temp != NULL) {
                if (data_temp->Id_Proc == temp->Id_Proc) {
                    break;
                }
                data_temp = data_temp->Next_Data;
            }
            al_draw_filled_rectangle(120 - slider + previous_x * scalar, 
                35 + length * 30, 
                120 - slider + (previous_x + temp->Exec_Cycles * 6) * scalar, 
                55 + length * 30, 
                al_map_rgb(data_temp->R, data_temp->G, data_temp->B));
            al_draw_rectangle(120 - slider + previous_x * scalar, 
                35 + length * 30, 
                120 - slider + (previous_x + temp->Exec_Cycles * 6) * scalar, 
                55 + length * 30 , 
                al_map_rgb(0, 0, 0), 1);
        }
        previous_x += temp->Exec_Cycles * 6;
        temp = temp->Next_Cycle;
    }

    al_draw_text(font, al_map_rgb(0, 0, 0), 306, 90 + length * 30, 0, "Time (s)");

    al_draw_filled_rectangle(0, 0, 119, 210, al_map_rgb(255, 255, 255));

    i = 0;
    while(i < length) {
        strcpy(temp_title, "Marciano \0");
        sprintf(str, "%d", i + 1);
        strcat(temp_title, str);
        al_draw_text(font, al_map_rgb(0, 0, 0), 30, 45 + i * 30, 0, temp_title);
        i++;
    }

    if (mode_r == 0) {
        al_draw_text(font, al_map_rgb(0, 0, 0), 30, 45 + i * 30, 0, "RMS");
    }
    else {
        al_draw_text(font, al_map_rgb(0, 0, 0), 30, 45 + i * 30, 0, "EDF");
    }

    al_flip_display();
}

void show_report() {
    /* Eliminar */
    // srand(time(NULL));
    /*int cycles_ids[] = {0, 1, 2, 0, 2, 1, 2, 0, -1, -1, -1, -1, -1, 0, 1};
    int cycles_num[] = {1, 2, 3, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2};
    for (int i = 0; i < 15; i++) {
        add_executed_cycle(cycles_ids[i], cycles_num[i]);
    }*/

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
