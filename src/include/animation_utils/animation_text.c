#ifndef ANIMATION_TEXT_C
#define ANIMATION_TEXT_C

#include <pokeagb/pokeagb.h>
#include "animation_text.h"
#include "../agb_debug/debug.c"


#define END_TEXT_FRAME {.frame_start = 0xFFFFFFFF}

void display_text(struct text_keyframe *keyframe){
    rboxid_clean (keyframe->textbox_id, true);
    rboxid_print (keyframe->textbox_id, keyframe->font, keyframe->x, keyframe->y, keyframe->color, keyframe->speed, keyframe->text);
    rboxid_update(keyframe->textbox_id, 3);
    rboxid_tilemap_update(keyframe->textbox_id);
}

#endif