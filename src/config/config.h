#ifndef CONFIG_H
#define CONFIG_H

#include "../assets.h" 
#include "../include/animation_utils/animation_utils.h" 

#define ANIMATION_STATE_ADDR 0x0202402C //default 0x0202402C: opponents party
#define ANIMATION_LEN 241 //in frames

struct scrolling_keyframe scrolling_keyframes[] = {
    {
        .bg_id=0,
        .frame_start = 0,
        .axis=X,
        .speed=1,
        .distance=0,
        .scroll_mode=ADD
    }, {
        .bg_id=1,
        .frame_start = 0,
        .axis=X,
        .speed=1<<3,
        .distance=0,
        .scroll_mode=ADD
    }, {
        .bg_id=2,
        .frame_start = 0,
        .axis=X,
        .speed=1<<5,
        .distance=0,
        .scroll_mode=ADD
    }, {
        .bg_id=3,
        .frame_start = 0,
        .axis=X,
        .speed=1<<7,
        .distance=0,
        .scroll_mode=ADD
    },
    END_SCROLLING_FRAME
};


extern struct drawing_keyframe drawing_keyframes[];

#endif