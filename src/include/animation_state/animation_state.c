#ifndef ANIMATION_STATE_C
#define ANIMATION_STATE_C

#include "animation_state.h"

void init_animation_state(){
    animation_state->curr_frame = 0;
    animation_state->curr_drawing_keyframe_i = 0;
    animation_state->curr_scrolling_keyframe_i = 0;
    animation_state->started = false;
    animation_state->ended = false;
}

#endif