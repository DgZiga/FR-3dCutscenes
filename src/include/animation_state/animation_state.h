#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H

#include <pokeagb/types.h>
#include "../config/config.h"

struct animation_state {
    u32 curr_frame;
    u32 curr_scrolling_keyframe_i;
    u32 curr_drawing_keyframe_i;
    bool started;
    bool ended;
};

struct animation_state *animation_state = (struct animation_state *)ANIMATION_STATE_ADDR;

void init_animation_state();

#endif