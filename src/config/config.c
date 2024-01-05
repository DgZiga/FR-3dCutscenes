
#ifndef CONFIG_C
#define CONFIG_C

#include "config.h"

#include "../built_graphics/bg0_0.h"
#include "../built_graphics/bg1_0.h"
#include "../built_graphics/bg2_0.h"
#include "../built_graphics/bg3_0.h"
#include "../built_graphics/bg2_60.h"
#include "../built_graphics/bg2_120.h"
struct drawing_keyframe drawing_keyframes[] = {{  
            .bgid=0,
            .asset=ASSET(0,0),
            .frame_start=0
        },{  
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
        },{  
            .bgid=2,
            .asset=ASSET(2,60),
            .frame_start=60
        },{  
            .bgid=2,
            .asset=ASSET(2,120),
            .frame_start=120
        },END_DRAWING_FRAME};

#endif
