#include "standard.h"

void move_marcian(MARCIAN *marcian)
{
    if (marcian->energy > 0 && steps > 0)
    {
        int pos_x = marcian->pos_x / 32;
        int pos_y = marcian->pos_y / 32;
        printf("Energy: %d, address: %c, marcian: %d\n", marcian->energy, marcian->direction, marcian->id);
        printf("Marcian: (%d, %d); Current(%d, %d)\n", pos_x, pos_y, current_x, current_y);
        switch (marcian->direction)
        {
        case 'u':
            marcian->pos_y -= 4;
            steps -= 1;
            sleep(0.3);
            break;
        case 'd':
            marcian->pos_y += 4;
            steps -= 1;
            sleep(0.3);
            break;
        case 'l':
            marcian->pos_x -= 4;
            steps -= 1;
            sleep(0.3);
            break;
        case 'r':
            marcian->pos_x += 4;
            steps -= 1;
            sleep(0.3);
            break;
        default:
            break;
        }
    }
}

bool check_movement(MARCIAN *marcian, char movement)
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
            printf("Posición en x: %f\n", marcian->pos_x - 32);
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
    if (pos_y >= 0 && pos_y <= 544 && pos_x >= 0 && pos_x <= 544)
    {
        if (maze[pos_y][pos_x] == movement || maze[pos_y][pos_x] == 'f')
        {
            current_x = marcian->pos_x / 32;
            current_y = marcian->pos_y / 32;

            maze[pos_y][pos_x] = movement;
            maze[current_y][current_x] = 'f';
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

void set_address(MARCIAN *marcian)
{
    srand(time(NULL));
    int pos;
    char next;

    while (true)
    {
        pos = rand() % 4;
        next = addresses[pos];
        if (check_movement(marcian, next))
        {
            marcian->direction = next;
            break;
        }
    }
}

void make_movement(MARCIAN *marcian)
{
    int pos_x = marcian->pos_x / 32;
    int pos_y = marcian->pos_y / 32;
    printf("ID: %d, Address: %c\n", marcian->id, marcian->direction);
    if (marcian->direction == 'c' || pos_x != current_x || pos_y != current_y)
    {

        if (marcian->direction != 'c' && check_movement(marcian, marcian->direction))
        {
            move_marcian(marcian);
        }
        else
        {
            set_address(marcian);
            move_marcian(marcian);
        }
    }
    else
    {
        move_marcian(marcian);
    }
}