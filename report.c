#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>

ALLEGRO_FONT* font;
int proc_len = 3;
int mode = 0;
int slider = 0;
int slider_state = 0;
int n_10 = 10;

void draw_window() {
    al_clear_to_color(al_map_rgb(255, 255, 255));
    if (slider_state == 1 && slider < (n_10 - 8) * 60) {
        slider += 2;
    }
    else if (slider_state == -1 && slider > 0) {
        slider -= 2;
    }
    char temp_title[25];
    char str[10];
    int i = 0;
    while(i < n_10) {
        sprintf(str, "%d", i * 10);
        al_draw_line(120 + i * 60 - slider, 30, 
            120 + i * 60 - slider, 150, al_map_rgb(0, 0, 0), 1);
        al_draw_text(font, al_map_rgb(0, 0, 0), 120 + i * 60 - slider, 150, 0, str);
        i++;
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
