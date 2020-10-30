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

    char energy_number[3];
    char period_number[3];
    sprintf(energy_number, "%d", new_energy);
    sprintf(period_number, "%d", new_period);

    float x_center = 544 + (window_width - 544) / 2.0f;

    // Title game mode
    al_draw_bitmap(marcian_image, x_center - 32, window_height / 2.0f - 40 * 4 - 72, 0);
    if (game_mode == 'a')
    {
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(a_mode), window_height / 2.0f - 30 * 4, 0, a_mode);
    }
    else
    {
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(m_mode), window_height / 2.0f - 30 * 4, 0, m_mode);
    }

    if (game_mode == 'm')
    {
        // Guide
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(modify_values_text), window_height / 2.0f + 30 * 4, 0, modify_values_text);
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(up_down_text), window_height / 2.0f + 30 * 4 + 10, 0, up_down_text);
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(r_l_text), window_height / 2.0f + 30 * 4 + 20, 0, r_l_text);
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(add_space), window_height / 2.0f + 30 * 4 + 40, 0, add_space);

        // Energy and period text
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(energy_text) - 10, window_height / 2.0f, 0, energy_text);
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center + 4 * sizeof(energy_number) + 10, window_height / 2.0f, 0, energy_number);
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(period_text) - 10, window_height / 2.0f + 10 * 4, 0, period_text);
        al_draw_text(font, al_map_rgb(255, 255, 255), x_center + 4 * sizeof(period_number) + 10, window_height / 2.0f + 10 * 4, 0, period_number);
    }

    // Finish
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(finish_text), window_height / 2.0f + 30 * 4 + 55, 0, finish_text);
}

void draw_marcians()
{
    for (int i = 0; i < length; i++)
    {
        al_draw_scaled_bitmap(marcian_image, 0, 0, imageWidth, imageHeight, marcians[i].pos_x, marcians[i].pos_y, imageWidth * 0.5, imageHeight * 0.5, 0);
    }
}

void draw_menu()
{

    float x_center = window_width / 2.0f;

    al_draw_bitmap(marcian_image, x_center - 32, window_height / 2.0f - 20 * 4 - 72, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(title), window_height / 2.0f - 20 * 4, 0, title);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(a_mode_guide), window_height / 2.0f, 0, a_mode_guide);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(m_mode_guide), window_height / 2.0f + 10 * 4, 0, m_mode_guide);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(exit_text), window_height / 2.0f + 30 * 4 + 55, 0, exit_text);
}

void draw_automatic_menu()
{
    char energy_number[3];
    char period_number[3];
    float x_center = window_width / 2.0f;
    sprintf(energy_number, "%d", new_energy);
    sprintf(period_number, "%d", new_period);

    al_draw_bitmap(marcian_image, window_width / 2.0f - 32, window_height / 2.0f - 20 * 4 - 72, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), window_width / 2.0f - 4 * sizeof(add_marcian_text), window_height / 2.0f - 20 * 4, 0, add_marcian_text);

    // Guide
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(modify_values_text), window_height / 2.0f + 30 * 4, 0, modify_values_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(up_down_text), window_height / 2.0f + 30 * 4 + 10, 0, up_down_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(r_l_text), window_height / 2.0f + 30 * 4 + 20, 0, r_l_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(start_space), window_height / 2.0f + 30 * 4 + 40, 0, start_space);

    // Energy and period text
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(energy_text) - 10, window_height / 2.0f, 0, energy_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center + 4 * sizeof(energy_number) + 10, window_height / 2.0f, 0, energy_number);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(period_text) - 10, window_height / 2.0f + 10 * 4, 0, period_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center - 4 * sizeof(exit_text), window_height / 2.0f + 30 * 4 + 55, 0, exit_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), x_center + 4 * sizeof(period_number) + 10, window_height / 2.0f + 10 * 4, 0, period_number);
}

void initialize_variables()
{

    // Window size
    window_height = 544;
    window_width = 832;
    length = 0;

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

    steps = 8;

    imageWidth = al_get_bitmap_width(marcian_image);
    imageHeight = al_get_bitmap_height(marcian_image);

    current_window = 'm';
    running = true;
    new_energy = 0;
    new_period = 0;

    head = NULL;
    Offsets = NULL;
    Offsets_len = 0;
    global_cycle = 0;
}

void add_new_marcian()
{
    if (new_energy != 0 && new_period != 0)
    {
        if (new_energy < new_period)
        {
            marcians[length] = (MARCIAN){.id = 1, .pos_x = 32, .pos_y = 32, .energy = new_energy, .period = new_period, .direction = 'c'};
            printf("New marcian added with energy: %d and period: %d\n", new_energy, new_period);

            if (Offsets == NULL) {
                Offsets = (int*)malloc(sizeof(int));
                *Offsets = 0;
                Offsets_len++;
            }
            else if (game_mode == 'm') {
                update_offsets(current_cycle);
                current_cycle = 0;
                if (create_offset() == -1) {
                    printf("Couldn't create the new offset!\n");
                    keep_execution = false;
                    running = false;
                    return;
                }
            }
            head = add_node(head, length, new_energy, new_period, Offsets + Offsets_len - 1);
            add_data(length, global_cycle, new_energy, new_period);
            length++;
            // print_list(head);

            if (game_mode == 'm') {
                multiple = lcm(head);
                pthread_t* threads_temp = (pthread_t*)realloc(threads, length);
                if (threads_temp == NULL) { 
                    printf("Couldn't create the new thread!\n");
                    keep_execution = false;
                    running = false;
                    return;
                } 
                else {
                    threads = threads_temp;
                }
                node_p* temp = head;
                while (temp->Id != length - 1) {
                    temp = temp->Next_Process;
                }
                pthread_create(threads + length - 1, NULL, exec_thread, (void *) temp);
            }

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
            if (current_window == 'g') {
                current_window = 'r';
                running = false;
                keep_execution = false;
            }
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
                printf("Current mode manual mode: %c\n", game_mode);
                // Create threads
                start_threads();
            }
            break;
        case ALLEGRO_KEY_SPACE:
            if (current_window == 'a')
            {
                if (length > 0)
                {
                    current_window = 'g';
                    game_mode = 'a';
                    // Create threads
                    start_threads();
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

        else if (current_window == 'a')
        {
            draw_automatic_menu();
        }
        else if (current_window == 'g')
        {

            if (!keep_execution) {
                current_window = 'r';
                running = false;
            }
            else {
                draw_background_game();

                if (length > 0)
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
                        if (current_marcian >= length)
                            current_marcian = 0;
                    }
                    if (current_marcian == length)
                        current_marcian = 0;
                }
            }
        }

        al_flip_display();
    }

    if (current_window == 'r') {
        /* Join threads */

        pthread_join(*planner, NULL);
        
        for (int i = 0; i < length; i++) {
            pthread_join(*(threads + i), NULL);
        }
    }

    al_destroy_bitmap(marcian_image);
    al_destroy_bitmap(background);
    al_destroy_bitmap(rock);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    if (current_window == 'r') {
        /* Show report */

        mode_r = mode;

        show_report();
    }

    return 0;
}