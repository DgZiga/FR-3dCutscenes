
#ifndef CONFIG_C
#define CONFIG_C

#include "config.h"

#include "../built_graphics/bg1_0.h"
#include "../built_graphics/bg2_0.h"
#include "../built_graphics/bg3_0.h"
struct drawing_keyframe drawing_keyframes[] = {{  
            .bgid=1,
            .asset=ASSET(1,0),
            .frame_start=0
        },{  
            .bgid=2,
            .asset=ASSET(2,0),
            .frame_start=0
        },{  
            .bgid=3,
            .asset=ASSET(3,0),
            .frame_start=0
        },END_DRAWING_FRAME};

#endif
