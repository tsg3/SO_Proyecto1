#include "movements.c"

void draw_background()
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

void draw_marcians(ALLEGRO_BITMAP *marcian)
{
    for (int i = 0; i < 3; i++)
    {
        al_draw_scaled_bitmap(marcian, 0, 0, imageWidth, imageHeight, marcians[i].pos_x, marcians[i].pos_y, imageWidth * 0.5, imageHeight * 0.5, 0);
    }
}

void set_cell_maze(MARCIAN *marcian)
{
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

    ALLEGRO_BITMAP *marcian_image = al_load_bitmap("assets/marcian2/marcian00.png");
    background = al_load_bitmap("assets/background.png");
    rock = al_load_bitmap("assets/rock.png");

    if (!marcian_image || !background)
    {
        printf("couldn't load mysha\n");
        return 1;
    }

    float scale = 0.5;

    marcians[0] = (MARCIAN){.id = 1, .pos_x = 32, .pos_y = 32, .energy = 4, .period = 9, .direction = 'c'};
    marcians[1] = (MARCIAN){.id = 2, .pos_x = 32, .pos_y = 32, .energy = 4, .period = 9, .direction = 'c'};
    marcians[2] = (MARCIAN){.id = 3, .pos_x = 32, .pos_y = 32, .energy = 4, .period = 9, .direction = 'c'};

    int current = 0;
    steps = 8;

    imageWidth = al_get_bitmap_width(marcian_image);
    imageHeight = al_get_bitmap_height(marcian_image);

    al_start_timer(timer);

    while (1)
    {

        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        make_movement(&marcians[current]);

        al_clear_to_color(al_map_rgb(185, 75, 36));

        draw_background();
        draw_marcians(marcian_image);
        if (steps == 0)
        {
            marcians[current].energy--;
            marcians[current].direction = 'c';
            steps = 8;
        }
        if (marcians[current].energy == 0)
        {
            marcians[current].energy = 4;
            current++;
        }
        if (current == 3)
            current = 0;

        al_flip_display();
    }

    al_destroy_bitmap(marcian_image);
    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}