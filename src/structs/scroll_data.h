#ifndef SCROLL_DATA_H
#define SCROLL_DATA_H

#include <pokeagb/types.h>

enum scroll_direction{
    UP, DOWN, LEFT, RIGHT
};

struct scroll_data {
    u32 start;
    enum scroll_direction direction;
    u32 speed;
};

#endif