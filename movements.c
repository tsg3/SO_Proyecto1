#include "report.c"

void move_marcian(node_p *marcian)
{
    if (marcian->Current_Energy > 0 && steps > 0)
    {
        // int pos_x = marcian->pos_x / 32;
        // int pos_y = marcian->pos_y / 32;
        // printf("Energy: %d, address: %c, marcian: %d\n", marcian->energy, marcian->direction, marcian->id);
        // printf("Marcian: (%d, %d); Current(%d, %d)\n", pos_x, pos_y, current_x, current_y);
        switch (marcian->direction)
        {
        case 'u':
            marcian->pos_y -= 32;
            // steps -= 1;
            break;
        case 'd':
            marcian->pos_y += 32;
            // steps -= 1;
            break;
        case 'l':
            marcian->pos_x -= 32;
            // steps -= 1;
            break;
        case 'r':
            marcian->pos_x += 32;
            // steps -= 1;
            break;
        default:
            break;
        }
    }
}

bool check_movement(node_p *marcian, char movement, bool checking)
{
    int pos_y;
    int pos_x;
    switch (movement)
    {
    case 'u':
        if (marcian->pos_y - 4 > 0)
        {
            pos_y = (marcian->pos_y - 4) / 32;
            pos_x = marcian->pos_x / 32;
        }
        break;
    case 'd':
        if (marcian->pos_y + 4 < 544)
        {
            pos_y = (marcian->pos_y + 32 + 4) / 32;
            pos_x = marcian->pos_x / 32;
        }
        break;
    case 'l':
        if (marcian->pos_x - 32 > 0)
        {
            // printf("Posición en x: %f\n", marcian->pos_x - 32);
            pos_y = marcian->pos_y / 32;
            pos_x = (marcian->pos_x - 4) / 32;
        }
        break;
    case 'r':
        if (marcian->pos_x + 4 < 544)
        {
            pos_y = marcian->pos_y / 32;
            pos_x = (marcian->pos_x + 32 + 4) / 32;
        }
        break;
    }
    // printf("(%.1f, %.1f) -> %c -> (%d, %d)\n", marcian->pos_x, marcian->pos_y, movement, pos_x, pos_y);
    if (pos_y >= 0 && pos_y <= 16 && pos_x >= 0 && pos_x <= 16)
    {
        if (maze[pos_y][pos_x] == 'f' || maze[pos_y][pos_x] == movement)
        {
            // printf("curr : %c %d %d\n", maze[current_y][current_x], current_x, current_y);
            // printf("next : %c %d %d\n", maze[pos_y][pos_x], pos_y, pos_x);
            if(!checking){
                current_x = marcian->pos_x / 32;
                current_y = marcian->pos_y / 32;

                maze[current_y][current_x] = 'f';
                maze[pos_y][pos_x] = movement;
            }
            // printf("New curr : %c %d %d\n", maze[current_y][current_x], current_x, current_y);
            // printf("New next : %c %d %d\n", maze[pos_y][pos_x], pos_y, pos_x);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void set_address(node_p *marcian)
{
    // srand(time(NULL));
    int pos;
    char next;

    while (true)
    {
        pos = rand() % 4;
        next = addresses[pos];
        if (check_movement(marcian, next, false))
        {
            marcian->direction = next;
            break;
        }
    }
}

bool check_options(node_p* martian) {
    if (martian->direction == 'u' || martian->direction == 'd')
        if (check_movement(martian, 'l', true) || check_movement(martian, 'r', true))
            return true;
    if (martian->direction == 'l' || martian->direction == 'r')
        if (check_movement(martian, 'd', true) || check_movement(martian, 'u', true))
            return true;
    return false;
}

bool theres_options(node_p* martian) {
    if (check_movement(martian, 'u', true) || check_movement(martian, 'd', true)
        || check_movement(martian, 'l', true) || check_movement(martian, 'r', true))
        return true;
    return false;
}

void make_movement(node_p *marcian)
{
    int pos_x = marcian->pos_x / 32;
    int pos_y = marcian->pos_y / 32;
    // printf("Address: %c\n", marcian->direction);
    // if (marcian->direction == 'c' || pos_x != current_x || pos_y != current_y)
    // {
    if(theres_options(marcian)){
        if (marcian->direction != 'c' && check_options(marcian)) {
            set_address(marcian);
            move_marcian(marcian);
        }
        else if (marcian->direction != 'c' && check_movement(marcian, marcian->direction, false))
        {
            move_marcian(marcian);
        }
        else
        {
            set_address(marcian);
            move_marcian(marcian);
        }
    }
    // }
    // else
    // {
    //     move_marcian(marcian);
    // }
}