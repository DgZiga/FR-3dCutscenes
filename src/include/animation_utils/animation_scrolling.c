#ifndef ANIMATION_SCROLLING_C
#define ANIMATION_SCROLLING_C

#include "animation_scrolling.h"

void task_scroll(u8 task_id);

void start_scroll(struct scrolling_keyframe scrolling_keyframe){
    u8 task_id = task_add((TaskCallback)(task_scroll), 0);
    tasks[task_id].priv[0] = scrolling_keyframe.bg_id;

    //game freak routines for some reason require the number to be lsh 8
    u32 delta = scrolling_keyframe.pixel_speed << 8;
    u32 max_distance = scrolling_keyframe.pixel_distance << 8;
    //pack u32 in 2 u16s
    tasks[task_id].priv[1] = delta & 0xFFFF;
    tasks[task_id].priv[2] = (delta >> 16) & 0x7FFF; //0x7FFF and not 0xFFFF to remove sign bit
    //pack u32 in 2 u16s
    tasks[task_id].priv[3] = max_distance & 0xFFFF;
    tasks[task_id].priv[4] = (max_distance >> 16) & 0x7FFF; //0x7FFF and not 0xFFFF to remove sign bit

    tasks[task_id].priv[5] = scrolling_keyframe.scroll_mode; //enum scroll_mode: 0 = override, 1 = add, 2 = sub
    tasks[task_id].priv[6] = scrolling_keyframe.axis; //axis

}


#include "../agb_debug/debug.c"

void task_scroll(u8 task_id){
    //read task vars
    struct Task curr_task = tasks[task_id];
    u8 bgid = curr_task.priv[0];
    //unpack 2 u16s in 1 u32
    u16 delta_low = curr_task.priv[1];
    u16 delta_high = curr_task.priv[2];
    u32 delta = delta_low | (delta_high << 16);
    //unpack 2 u16s in 1 u32
    u16 max_distance_low = curr_task.priv[3];
    u16 max_distance_high = curr_task.priv[4];
    u32 max_distance = max_distance_low | (max_distance_high << 16);

    enum scroll_mode mode = curr_task.priv[5];
    enum axis axis = curr_task.priv[6];

    bool delete_task = false;
    struct BgConfig2 bg_cfg = bg_config2[bgid];
    //read curr distance depending on axis (there probably is a smarter way to do this)
    u32 *distance_ptr = (u32*)0;
    if(axis == X){
        distance_ptr = &(bg_cfg.x);
    } else if (axis == Y){
        distance_ptr = &(bg_cfg.y);
    }
    u32 distance = *distance_ptr;
    //dprintf("mode 0x%x\n, axis 0x%x, \n", mode, axis);
    //dprintf("delta 0x%x, dist 0x%x\n", delta, distance);
    
    if(max_distance != 0){
        if(distance >= max_distance){
            delta = max_distance;
            mode = SET;
            delete_task = true;
        }
    }

    if(axis == X) {
        bgid_mod_x_offset(bgid, delta, mode);
    } else if (axis == Y) {
        bgid_mod_y_offset(bgid, delta, mode);
    }

    if(delete_task){
        task_del(task_id);
    }
}


#endif