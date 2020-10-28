

const char maze[17][17] = {
    {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'},
    {'i', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'o'},
    {'i', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'o', 'f', 'f', 'o', 'o', 'o', 'o', 'o', 'o', 'f', 'f', 'o', 'o', 'o'},
    {'o', 'f', 'f', 'o', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'o', 'o', 'o', 'o', 'f', 'f', 'o'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'o', 'f', 'f', 'o', 'o', 'o', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'o', 'o', 'o', 'f', 'f', 'o', 'f', 'f', 'o', 'o', 'o', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'o'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'f', 'f', 'o', 'f', 'f', 'o', 'f', 'f', 'o', 'o', 'o'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'm'},
    {'o', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'o', 'f', 'f', 'f', 'f', 'm'},
    {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'}};

typedef struct MARCIAN
{
    float pos_x;
    float pos_y;
    int energy;
    int period;
    char direction;
} MARCIAN;
