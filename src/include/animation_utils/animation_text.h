#ifndef ANIMATION_TEXT_H
#define ANIMATION_TEXT_H

#include <pokeagb/pokeagb.h>
#define rgb5(r, g, b) (u16)((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10))

struct text_keyframe {
    u32 frame_start;
    u8 textbox_id;
    pchar* text;
    u8 font;
    u8 x;
    u8 y;
    struct TextColor *color;
    u8 speed; //0=instant, the higher the slower text is displayed on screen
};

#define END_TEXT_FRAME {.frame_start = 0xFFFFFFFF}

void display_text(struct text_keyframe *text_keyframe);

#endif