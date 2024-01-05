#ifndef ANIMATION_SCROLLING_H
#define ANIMATION_SCROLLING_H

#include <pokeagb/pokeagb.h>

enum axis {X,Y};
enum scroll_mode {SET, ADD, SUB};

struct scrolling_keyframe {
    u32 frame_start;
    u16 bg_id;
    //0xFF is a single pixel
    u32 speed;
    //0xFF is a single pixel
    u32 distance;
    enum scroll_mode scroll_mode;
    enum axis axis;
};

#define END_SCROLLING_FRAME {.frame_start = 0xFFFFFFFF}

void start_scroll(struct scrolling_keyframe scrolling_keyframe);

#endif