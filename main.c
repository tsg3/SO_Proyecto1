#include "movements.c"

void draw_background_game()
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

void draw_marcians()
{
    for (int i = 0; i < lenght; i++)
    {
        al_draw_scaled_bitmap(marcian_image, 0, 0, imageWidth, imageHeight, marcians[i].pos_x, marcians[i].pos_y, imageWidth * 0.5, imageHeight * 0.5, 0);
    }
}


void draw_menu()
{
    al_draw_bitmap(marcian_image, window_width/2.0f - 32, window_height/2.0f - 20*4 - 72, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), window_width/2.0f - 64, window_height/2.0f - 20*4, 0, "STUPID MARCIANS");

}


void initialize_variables()
{

    // Window size
    window_height = 544;
    window_width = 720;
    lenght = 0;

    timer = al_create_timer(1.0 / 30.0);
    queue = al_create_event_queue();
    disp = al_create_display(window_width, window_height);
    font = al_create_builtin_font();
    // Carga sprite
    if (!al_init_image_addon())
    {
        printf("couldn't initialize image addon\n");
        exit(1);
    }
    marcian_image = al_load_bitmap("assets/marcian2/marcian00.png");
    background = al_load_bitmap("assets/background.png");
    rock = al_load_bitmap("assets/rock.png");

    if (!marcian_image || !background)
    {
        printf("couldn't load mysha\n");
        exit(1);
    }

    float scale = 0.5;

    marcians[0] = (MARCIAN){.id = 1, .pos_x = 32, .pos_y = 32, .energy = 4, .period = 9, .direction = 'c'};
    marcians[1] = (MARCIAN){.id = 2, .pos_x = 32, .pos_y = 32, .energy = 4, .period = 9, .direction = 'c'};
    marcians[2] = (MARCIAN){.id = 3, .pos_x = 32, .pos_y = 32, .energy = 4, .period = 9, .direction = 'c'};

    steps = 8;

    imageWidth = al_get_bitmap_width(marcian_image);
    imageHeight = al_get_bitmap_height(marcian_image);

    current_window = 'm';
    running = true;
    new_energy = 0;
    new_period = 0;
}

void add_new_marcian()
{
    if (new_energy != 0 && new_period != 0)
    {
        if (new_energy < new_period)
        {
            marcians[lenght] = (MARCIAN){.id = 1, .pos_x = 32, .pos_y = 32, .energy = new_energy, .period = new_period, .direction = 'c'};
            lenght++;
            printf("New marcian added with energy: %d and period: %d\n", new_energy, new_period);
            new_period = 0;
            new_energy = 0;
        }
    }
    else
    {
        printf("Energy and period must be greater than zero\n");
    }
}

void validate_key()
{
    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_X:
            if (current_window != 'g')
                current_window = 'r';
            else
                running = false;
            break;

        case ALLEGRO_KEY_A:
            if (current_window == 'm')
                current_window = 'a';
            else if (current_window == 'a')
                add_new_marcian();
            break;

        case ALLEGRO_KEY_S:
            if (current_window == 'm')
            {
                current_window = 'g';
                game_mode = 'm';
            }
            break;
        case ALLEGRO_KEY_SPACE:
            if (current_window == 'a')
            {
                if (lenght > 0)
                {
                    current_window = 'g';
                    game_mode = 'a';
                }
            }
            else if (current_window == 'g' && game_mode == 'm')
            {
                printf("Current mode manual\n");
                add_new_marcian();
            }
            break;
        case ALLEGRO_KEY_UP:
            if ((game_mode == 'm' && current_window == 'g') || current_window == 'a')
            {
                new_period++;
            }
            break;
        case ALLEGRO_KEY_DOWN:
            if ((game_mode == 'm' && current_window == 'g') || current_window == 'a')
            {
                if (new_period > 0)
                    new_period--;
            }
            break;
        case ALLEGRO_KEY_RIGHT:
            if ((game_mode == 'm' && current_window == 'g') || current_window == 'a')
            {
                new_energy++;
            }
            break;
        case ALLEGRO_KEY_LEFT:
            if ((game_mode == 'm' && current_window == 'g') || current_window == 'a')
            {
                if (new_energy > 0)
                    new_energy--;
            }
            break;
        default:
            break;
        }
    }
}

int main()
{
    al_init();
    al_install_keyboard();

    // Inicializar las variables
    initialize_variables();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;

    al_start_timer(timer);
    // Marcian manager
    int current_marcian = 0;

    // Game loop
    while (running)
    {

        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        al_clear_to_color(al_map_rgb(0, 0, 0));

        validate_key();

        if (current_window == 'm')
            draw_menu();
        else if (current_window == 'g')
        {

            draw_background_game();

            if (lenght > 0)
            {
                make_movement(&marcians[current_marcian]);

                draw_marcians(marcian_image);

                // Validaciones temporales
                if (steps == 0)
                {
                    marcians[current_marcian].energy--;
                    marcians[current_marcian].direction = 'c';
                    steps = 8;
                }
                if (marcians[current_marcian].energy == 0)
                {
                    marcians[current_marcian].energy = 4;
                    current_marcian++;
                    if (&marcians[current_marcian] == NULL)
                        current_marcian = 0;
                }
                if (current_marcian == 3)
                    current_marcian = 0;
            }
        }

        al_flip_display();
    }

    al_destroy_bitmap(marcian_image);
    al_destroy_bitmap(background);
    al_destroy_bitmap(rock);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}