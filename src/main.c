#ifndef MAIN_C
#define MAIN_C

#include "config.h"
#include "include/animation_state/animation_state.h"
#include <pokeagb/pokeagb.h>

int main();
POKEAGB_EXTERN void run_eventually_start(u8 task_id);

#define BY_ITEM(id)  \
void gui_##id##_by_item_run_eventually(){\
	main();\
}\
void gui_##id##_by_item_task(u8 task_id){\
	c2_exit_to_overworld_2_switch();\
	*run_eventually = gui_##id##_by_item_run_eventually;\
	run_eventually_start(task_id);\
}\
void gui_##id##_new_gui_by_item(){\
	task_add(gui_##id##_by_item_task, 0);\
}

BY_ITEM(1);

void CpuFastSet(void* src, void* dst, u32 mode)
{
    __asm__("swi 0xC");
}

int main(){
    fade_screen(0xFFFFFFFF, 0, 0, 16, 0x0000);
    void gui_handler(void);
    set_callback1(gui_handler);
    super.multi_purpose_state_tracker = 0;
	return 1;
}


#define CPUFSCPY 0
#define CPUFSSET 1
#define CPUModeFS(size, mode) ((size >> 2) | (mode << 24))

void handlers_clear() {
	vblank_handler_set(0);
	hblank_handler_set(0);
	set_callback1(0);
	set_callback2(0);
}

void reset_pal_settings() {
	pal_fade_control_and_dead_struct_reset();
	palette_bg_faded_fill_black();
	gpu_pal_allocator_reset();
	*gpu_pal_tag_search_lower_boundary = 0;
}


void reset_bg_settings() {
	overworld_free_bgmaps();
	gpu_tile_bg_drop_all_sets(0);
	bgid_mod_x_offset(0, 0, 0);
    bgid_mod_y_offset(0, 0, 0);
	bgid_mod_x_offset(1, 0, 0);
    bgid_mod_y_offset(1, 0, 0);
	bgid_mod_x_offset(2, 0, 0);
    bgid_mod_y_offset(2, 0, 0);
	bgid_mod_x_offset(3, 0, 0);
    bgid_mod_y_offset(3, 0, 0);
}

void reset_boxes() {
	remo_reset_acknowledgement_flags();
	rboxes_free();
}
void setup()
{
    // callbacks
    handlers_clear();
    // BGs
    reset_bg_settings();
    // pals
    reset_pal_settings();
    // objs
    obj_and_aux_reset_all();
    gpu_tile_obj_tags_reset();
    // VRAM clear
    u32 set = 0;
    CpuFastSet((void*)&set, (void*)ADDR_VRAM, CPUModeFS(0x10000, CPUFSSET));
    // tasks
    malloc_init((void*)0x2000000, 0x1C000);
    tasks_init();
}

#include "include/agb_debug/debug.c"

void c2_gui(){

    //do animation
    if(animation_state->started && !animation_state->ended){
        //dprintf("animating frame %x\n", animation_state->curr_frame);
        
        //process drawing
        struct drawing_keyframe *drawing_keyframe = &(drawing_keyframes[animation_state->curr_drawing_keyframe_i]);
        while(drawing_keyframe->frame_start == animation_state->curr_frame){
            //dprintf("drawing keyframe with index %x. (keyframe start is\n", animation_state->curr_drawing_keyframe_i);
            
            load_asset_to_bg(*drawing_keyframe);
            animation_state->curr_drawing_keyframe_i++;
            drawing_keyframe = &(drawing_keyframes[animation_state->curr_drawing_keyframe_i]);
        }
        //process scroll
        struct scrolling_keyframe scrolling_keyframe = scrolling_keyframes[animation_state->curr_scrolling_keyframe_i];
        if(scrolling_keyframe.frame_start == animation_state->curr_frame){
            start_scroll(scrolling_keyframe);
            animation_state->curr_scrolling_keyframe_i++;
        }
        animation_state->curr_frame++;

        if(animation_state->curr_frame > ANIMATION_LEN){
            animation_state->ended = true;
        }
    }

    obj_sync_superstate();
    objc_exec();
    process_palfade();
    task_exec();
    tilemaps_sync();
    // merge textbox and text tile maps
    remoboxes_upload_tilesets();
}
void vblank_cb_spq(){
	gpu_sprites_upload();
	copy_queue_process();
	gpu_pal_upload();
}

void gui_handler(){
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
				set_callback2(c2_gui);
				vblank_handler_set(vblank_cb_spq);
				set_callback1(gui_handler);
				
				super.multi_purpose_state_tracker++;;
			}
			break;
		case 1: { //Load backgground image, init rboxes and quest list
			//Load bg image
            animation_state->started = true;
            //load_asset_to_bg(drawing_keyframes[0]);
            //load_asset_to_bg(drawing_keyframes[1]);
            //load_asset_to_bg(drawing_keyframes[2]);
            //load_asset_to_bg(drawing_keyframes[3]);
			//init rboxes
			//rbox_init_from_templates(def.textboxes);
			
			super.multi_purpose_state_tracker++;
		}
			break;
		case 2: //fill rboxes
        
            if (!pal_fade_control.active) { //Wait for fadescreen to stop
				// clean boxes
				for (u32 i = 0; i < 5; ++i) {
					//rboxid_clear_pixels(i, 0);
				}
				
				super.multi_purpose_state_tracker++;
			}
		case 3: {//Display OWs 
			super.multi_purpose_state_tracker++;
		}
		case 4: // Show screen
            fade_screen(0xFFFFFFFF, 0, 16, 0, 0x0000);
            super.multi_purpose_state_tracker++;
			break;
        case 5: 

        if (!pal_fade_control.active) { //Wait for fadescreen to end, start animation
            if(!animation_state->started){
                //animation_state->started = true;
            }
        }
        break;
	}
	
	
}

#endif