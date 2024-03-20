#ifndef CONFIG_H
#define CONFIG_H

#include "../assets.h" 
#include "../include/animation_utils/animation_utils.h" 

#define ANIMATION_STATE_ADDR 0x0202402C //default 0x0202402C: opponents party
#define ANIMATION_LEN 601 //in frames

struct scrolling_keyframe scrolling_keyframes[] = {
    {
        .bg_id=0,
        .frame_start = 0,
        .axis=X,
        .speed=0,
        .distance=0,
        .scroll_mode=ADD
    }, {
        .bg_id=1,
        .frame_start = 0,
        .axis=X,
        .speed=1<<7,
        .distance=0,
        .scroll_mode=ADD
    }, {
        .bg_id=2,
        .frame_start = 0,
        .axis=X,
        .speed=1<<5,
        .distance=0,
        .scroll_mode=ADD
    }, {
        .bg_id=3,
        .frame_start = 0,
        .axis=X,
        .speed=1<<3,
        .distance=0,
        .scroll_mode=ADD
    },
    END_SCROLLING_FRAME
};

struct TextboxTemplate textboxes[] = {
                /*{
                    .bg_id = 0xFF, // marks the end of the tb array 
                },*/
                {
                    .bg_id=0,
                    .x=2,      //multiplied by 8
                    .y=1,      //starts from 4 pixels, multiplied by 8
                    .width=25,  //multiplied by 4
                    .height=5, //multiplied by 4
                    .pal_id=15,
                    .charbase=1
                },{
                    .bg_id = 0xFF, // marks the end of the tb array 
                }
            };

            
const u16 text_palettes[] = {
	rgb5(255, 0, 255), rgb5(248, 248, 248), rgb5(112, 112, 112),
	rgb5(0, 0, 0), rgb5(208, 208, 208), rgb5(76, 154, 38),
	rgb5(102, 194, 66), rgb5(168, 75, 76), rgb5(224, 114, 75),
	rgb5(180, 124, 41), rgb5(241, 188, 60), rgb5(255, 0, 255),
    rgb5(255, 0, 255), rgb5(255, 0, 255), rgb5(255, 133, 200), rgb5(64, 200, 248)}; //example palettes

struct TextColor text_color_black = {0, 3, 4}; //example palettes
struct TextColor text_color_white = {3, 1, 2}; //example palettes

extern pchar example_text_1[]; //defined in main.s
extern pchar example_text_2[]; //defined in main.s
struct text_keyframe text_keyframes[] = {
    {
        .frame_start = 20,
        .text = example_text_1,
        .textbox_id = 0, //position in textboxes array 
        .font = 0, //0 is default? Idk honestly
        .x = 0,
        .y = 0,
        .color = &text_color_black,
        .speed = 0 //instant
    }, {
        .frame_start = 80,
        .text = example_text_1,
        .textbox_id = 0, //position in textboxes array 
        .font = 1, //0 is default? Idk honestly
        .x = 0,
        .y = 0,
        .color = &text_color_black,
        .speed = 1 //slow
    }, {
        .frame_start = 200,
        .text = example_text_1,
        .textbox_id = 0, //position in textboxes array 
        .font = 3, //0 is default? Idk honestly
        .x = 1,
        .y = 1,
        .color = &text_color_white,
        .speed = 2 //slower
    },
    END_TEXT_FRAME
};

extern struct drawing_keyframe drawing_keyframes[];

#endif