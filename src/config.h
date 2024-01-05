#ifndef CONFIG_H
#define CONFIG_H

#include "assets.h" 
#include "include/animation_utils/animation_utils.h" 


#define ANIMATION_STATE_ADDR 0x0202402C //default 0x0202402C: opponents party
#define ANIMATION_LEN 181 //in frames

//ASSET(0,0)
const struct asset bg0_0_asset ={ .tiles_len = 728, .tiles = bg0_0Tiles, .map_len = 464, .map = bg0_0Map, .pals_len = 20, .pals = bg0_0Pal, };

ASSET(1,0)
ASSET(2,0)
ASSET(3,0)

struct scrolling_keyframe scrolling_keyframes[];



struct drawing_keyframe drawing_keyframes[] = {
    {
        .bgid=0,
        .asset=ASSET2(0,0),
        .frame_start=0
    },
    {
        .bgid=1,
        .asset=ASSET2(1,0),
        .frame_start=0
    },
    {
        .bgid=2,
        .asset=ASSET2(2,0),
        .frame_start=0
    },
    {
        .bgid=3,
        .asset=ASSET2(3,0),
        .frame_start=0
    },
    {
        .bgid=2,
        .asset=ASSET2(2,60),
        .frame_start=180
    },
    END_DRAWING_FRAME
};

#endif