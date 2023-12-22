#ifndef CONFIG_H
#define CONFIG_H

#include "structs/frame_data.h"
#include "structs/scroll_data.h" 
#include "structs/timeline_frame.h" 
#include "assets.h" 
#include "timeline.h" 

#define TIMELINE_SIZE 10
#define SCENE_LEN 2

ASSET(0,0)
ASSET(1,0)
ASSET(2,0)
ASSET(3,0)

struct frame_data bg_0_frame_data[] = {{.frame_id= 0, .start=0}};
struct frame_data bg_1_frame_data[] = {{.frame_id= 0, .start=0}};
struct frame_data bg_2_frame_data[] = {{.frame_id= 0, .start=0}};
struct frame_data bg_3_frame_data[] = {{.frame_id= 0, .start=0}};

struct scroll_data bg_0_scroll_data[] = {{.start=0, .direction=UP   , .speed=5}};
struct scroll_data bg_1_scroll_data[] = {{.start=0, .direction=DOWN , .speed=5}};
struct scroll_data bg_2_scroll_data[] = {{.start=0, .direction=LEFT , .speed=5}};
struct scroll_data bg_3_scroll_data[] = {{.start=0, .direction=RIGHT, .speed=5}};




#endif