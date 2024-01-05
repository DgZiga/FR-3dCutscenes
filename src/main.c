#ifndef MAIN_C
#define MAIN_C

#include "config.h"
#include "include/animation_state/animation_state.h"
#include "include/setup/setup.h"
#include <pokeagb/pokeagb.h>

void c2_animation();
void exit_anim();

int main(){
    fade_screen(0xFFFFFFFF, 0, 0, 16, 0x0000);
    void main_c1_handler(void);
    set_callback1(main_c1_handler);
    super.multi_purpose_state_tracker = 0;
	return 1;
}

void main_c1_handler(){
	struct FadeControl pal_fade_control = *((struct FadeControl *)0x02037ab8);

	switch(super.multi_purpose_state_tracker){
		case 0:
            if (!pal_fade_control.active) {
				setup(); 					//clear all graphics
				rboxes_free(); 				//clear rboxes
				bg_vram_setup(0, bg_config, 4);
				//Clear VRAM
				u32 set = 0;
				CpuFastSet((void*)&set, (void*)ADDR_VRAM, CPUModeFS(0x10000, CPUFSSET));
                //Init animation_state
                init_animation_state();
				//Set callbacks
				set_callback2(c2_animation);
				vblank_handler_set(vblank_cb_spq);
				set_callback1(main_c1_handler);
				
				super.multi_purpose_state_tracker++;;
			}
			break;
		case 1: //show screen
            fade_screen(0xFFFFFFFF, 0, 16, 0, 0x0000);
            super.multi_purpose_state_tracker++;
			break;
		case 2:  //Start animation
            animation_state->started = true;
			super.multi_purpose_state_tracker++;
			break;
        case 3: 
            if (animation_state->ended) { 
                set_callback1(exit_anim);
                super.multi_purpose_state_tracker = 0;
                return;
            }
            break;
	}
	
}


void c2_animation(){
    //do animation
    if(animation_state->started && !animation_state->ended){        
        //process drawing
        struct drawing_keyframe *drawing_keyframe = &(drawing_keyframes[animation_state->curr_drawing_keyframe_i]);
        while(drawing_keyframe->frame_start == animation_state->curr_frame){
            load_asset_to_bg(*drawing_keyframe);
            animation_state->curr_drawing_keyframe_i++;
            drawing_keyframe = &(drawing_keyframes[animation_state->curr_drawing_keyframe_i]);
        }

        //process scroll
        struct scrolling_keyframe *scrolling_keyframe = &(scrolling_keyframes[animation_state->curr_scrolling_keyframe_i]);
        while(scrolling_keyframe->frame_start == animation_state->curr_frame){
            start_scroll(*scrolling_keyframe);
            animation_state->curr_scrolling_keyframe_i++;
            scrolling_keyframe = &(scrolling_keyframes[animation_state->curr_scrolling_keyframe_i]);
        }
        animation_state->curr_frame++;

        if(animation_state->curr_frame > ANIMATION_LEN){
            animation_state->ended = true;
        }
    }

    // housekeeping
    obj_sync_superstate();
    objc_exec();
    process_palfade();
    task_exec();
    tilemaps_sync();
    // merge textbox and text tile maps
    remoboxes_upload_tilesets();
}


void exit_anim() {
    switch (super.multi_purpose_state_tracker) {
        case 0:
			fade_screen(0xFFFFFFFF, 0, 0, 16, 0x0000);
			gpu_sync_bg_hide(1);
			gpu_sync_bg_hide(0);	
			super.multi_purpose_state_tracker++;
            break;
        case 1:
            m4aMPlayVolumeControl(&mplay_BGM, 0xFFFF, 256);
            set_callback1(c1_overworld);
            set_callback2(c2_exit_to_overworld_2_switch);
			super.multi_purpose_state_tracker++;
            break;
		case 2:
            fade_screen(0xFFFFFFFF, 0, 16, 0, 0x0000);
			gpu_sync_bg_show(0);
			gpu_sync_bg_show(1);
			super.multi_purpose_state_tracker++;
		break;
	}
}

#endif