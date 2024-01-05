#ifndef ANIMATION_SCROLLING_H
#define ANIMATION_SCROLLING_H

#include <pokeagb/pokeagb.h>

enum axis {X,Y};
enum scroll_mode {SET, ADD, SUB};

struct scrolling_keyframe {
    u32 frame_start;
    u16 bg_id;
    u8 pixel_speed;
    u8 pixel_distance;
    enum scroll_mode scroll_mode;
    enum axis axis;
};

void start_scroll(struct scrolling_keyframe scrolling_keyframe);

#endif