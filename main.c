#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "standard.h"

ALLEGRO_BITMAP *background;
ALLEGRO_BITMAP *rock;
MARCIAN marcian1 = {0, 32, 3, 7, 'c'};
;
ALLEGRO_EVENT event;

void print_background()
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (maze[i][j] != 'o')
            {
                al_draw_bitmap(background, j * 32, i * 32, 0);
            }
            else
            {
                al_draw_bitmap(rock, j * 32, i * 32, 0);
            }
        }
    }
}

void move_marcian()
{

    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        printf("x: %f, y: %f\n", marcian1.pos_x, marcian1.pos_y);
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_UP:
            if (marcian1.pos_y - 4 > 0)
            {
                int pos_y = (marcian1.pos_y - 4) / 32;
                int pos_x = marcian1.pos_x / 32;
                int pos_x_l = (marcian1.pos_x + 32) / 32;
                if (maze[pos_y][pos_x] != 'o' && maze[pos_y][pos_x_l] != 'o')
                {
                    marcian1.pos_y -= 4;
                }
                else
                {
                    printf("Can't make this movement to Up ");
                }
            }
            break;
        case ALLEGRO_KEY_DOWN:
            if (marcian1.pos_y + 4 < 544)
            {
                int pos_y = (marcian1.pos_y + 32 + 4) / 32;
                int pos_x = marcian1.pos_x / 32;
                int pos_x_l = (marcian1.pos_x + 32) / 32;
                if (maze[pos_y][pos_x] != 'o' && maze[pos_y][pos_x_l] != 'o')
                {
                    marcian1.pos_y += 4;
                }
                else
                {
                    printf("Can't make this movement to Down");
                }
            }
            break;
        case ALLEGRO_KEY_LEFT:
            if (marcian1.pos_x - 4 > 0)
            {
                int pos_y = marcian1.pos_y / 32;
                int pos_x = (marcian1.pos_x - 4) / 32;
                int pos_y_d = (marcian1.pos_y + 32) / 32;
                if (maze[pos_y][pos_x] != 'o' && maze[pos_y_d][pos_x] != 'o')
                {
                    marcian1.pos_x -= 4;
                }
                else
                {
                    printf("Can't make this movement to Left");
                }
            }
            break;
        case ALLEGRO_KEY_RIGHT:
            if (marcian1.pos_x - 4 < 544)
            {
                int pos_y = marcian1.pos_y / 32;
                int pos_x = (marcian1.pos_x + 32 + 4) / 32;
                int pos_y_d = (marcian1.pos_y + 32) / 32;
                if (maze[pos_y][pos_x] != 'o' && maze[pos_y_d][pos_x] != 'o')
                {
                    marcian1.pos_x += 4;
                }
                else
                {
                    printf("Can't make this movement to Right");
                }
            }
            break;
        }
    }
}

int main()
{
    al_init();
    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(640, 544);
    ALLEGRO_FONT *font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;

    // Carga sprite
    if (!al_init_image_addon())
    {
        printf("couldn't initialize image addon\n");
        return 1;
    }

    ALLEGRO_BITMAP *marcian = al_load_bitmap("assets/marcian2/marcian00.png");
    background = al_load_bitmap("assets/background.png");
    rock = al_load_bitmap("assets/rock.png");

    if (!marcian || !background)
    {
        printf("couldn't load mysha\n");
        return 1;
    }

    float scale = 0.5;

    int imageWidth = al_get_bitmap_width(marcian);
    int imageHeight = al_get_bitmap_height(marcian);

    al_start_timer(timer);
    while (1)
    {
    
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
       
        move_marcian();

        al_clear_to_color(al_map_rgb(185, 75, 36));

        // Pintando el laberinto
        print_background();

        al_draw_scaled_bitmap(marcian, 0, 0, imageWidth, imageHeight, marcian1.pos_x, marcian1.pos_y, imageWidth * scale, imageHeight * scale, 0);

        al_flip_display();
    }

    al_destroy_bitmap(marcian);
    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}