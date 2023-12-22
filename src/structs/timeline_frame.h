#ifndef TIMELINE_FRAME_H
#define TIMELINE_FRAME_H

#include <pokeagb/types.h>

struct timeline_frame {
    u32 start;
    void (*func_ptr)();
};

#endif