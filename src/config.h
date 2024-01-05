#ifndef CONFIG_H
#define CONFIG_H

#include "assets.h" 
#include "include/animation_utils/animation_utils.h" 


#define ANIMATION_STATE_ADDR 0x0202402C //default 0x0202402C: opponents party
#define ANIMATION_LEN 181 //in frames

struct scrolling_keyframe scrolling_keyframes[] = {
    {
        .bg_id=0,
        .frame_start = 60,
        .axis=X,
        .pixel_speed=1,
        .pixel_distance=140,
        .scroll_mode=ADD
    },
    END_SCROLLING_FRAME
};


struct drawing_keyframe drawing_keyframes[] = {
    {
        .bgid=0,
        .asset=ASSET(0,0),
        .frame_start=0
    },
    {
        .bgid=1,
        .asset=ASSET(1,0),
        .frame_start=0
    },
    {
        .bgid=2,
        .asset=ASSET(2,0),
        .frame_start=0
    },
    {
        .bgid=3,
        .asset=ASSET(3,0),
        .frame_start=0
    },
    {
        .bgid=2,
        .asset=ASSET(2,60),
        .frame_start=180
    },
    END_DRAWING_FRAME
};

#endif