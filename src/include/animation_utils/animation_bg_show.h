#ifndef ANIMATION_BG_SHOW_H
#define ANIMATION_BG_SHOW_H

#include <pokeagb/pokeagb.h>
#include "../../assets.h"

extern struct BgConfig bg_config[4];

struct drawing_keyframe {
    u32 frame_start;
    u8 bgid;
    struct asset asset;
};

#define END_DRAWING_FRAME {.frame_start = 0xFFFFFFFF}

void load_asset_to_bg(struct drawing_keyframe drawing_keyframe);

#endif