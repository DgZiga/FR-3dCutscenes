#ifndef MAIN_C
#define MAIN_C

#include "config.h"
#include "timeline.h"
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

//char_base is multiplied by 0x4000 and added to 0x06000000. 
//map_base is multiplied by 0x800 and added to 0x06000000.
//size: 0 is 256x256, 1 is 512x256
//priority: 0-3, 0 is higher
//palette: 0 (16) or 1(256)
struct BgConfig bg_config[4] = { 
    {.padding=0, .b_padding=0, .priority=3, .palette=0, .size=0, .map_base=6 /* 6 = 0x03000*/, .character_base=0/*0x04000*/, .bgid=0, }, 
    {.padding=0, .b_padding=0, .priority=2, .palette=0, .size=0, .map_base=14/*14 = 0x0A000*/, .character_base=1/*0x08000*/, .bgid=1, }, 
    {.padding=0, .b_padding=0, .priority=1, .palette=0, .size=0, .map_base=22/*22 = 0x11000*/, .character_base=2/*0x0C000*/, .bgid=2, }, 
    {.padding=0, .b_padding=0, .priority=0, .palette=0, .size=0, .map_base=30/*30 = 0x18000*/, .character_base=3/*0x10000*/, .bgid=3, } };

struct BgConfig *get_bg_config_by_bg_id(u8 bgid){
    for (u8 i=0; i<sizeof(bg_config)/sizeof(bg_config[0]); i++){
        if(bg_config[i].bgid == bgid){
            return &bg_config[i];
        }
    }
    return &bg_config[5]; //break everything on purpose TODO handle this case
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

void c2_gui(){
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

u32 calc_char_start(struct BgConfig *bg_cfg){
    return ADDR_VRAM + (bg_cfg->character_base*0x4000);
}

void load_asset_to_bg(struct asset asset, u8 bgid){
    u8 pal_id = bgid; //just convenient
    struct BgConfig *bg_cfg = get_bg_config_by_bg_id(bgid);
    void *buffer = malloc(0x1000);
    gpu_pal_apply_compressed((void *)(asset.pals), pal_id * 16, 32);
    LZ77UnCompWram((void *)(asset.map), (void *)buffer);
    lz77UnCompVram((void *)(asset.tiles), (void *)calc_char_start(bg_cfg));
    bgid_set_tilemap(bgid, buffer);
    bgid_mark_for_sync(bgid);
    gpu_sync_bg_show(bgid);
}
void task_scroll(u8 task_id);

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
				//Set callbacks
				set_callback2(c2_gui);
				vblank_handler_set(vblank_cb_spq);
				set_callback1(gui_handler);
				
				super.multi_purpose_state_tracker++;;
			}
			break;
		case 1: { //Load backgground image, init rboxes and quest list
			//Load bg image
			load_asset_to_bg(bg0_0_asset, 0);
			load_asset_to_bg(bg1_0_asset, 1);
			load_asset_to_bg(bg2_0_asset, 2);
			load_asset_to_bg(bg3_0_asset, 3);
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
        case 5: //Input control NOTE: L and R ARE INVERTED IN POKEAGB
        if (!pal_fade_control.active) { //Wait for fadescreen to end
            switch (super.buttons_new_remapped) {
                case KEY_SELECT:
                    break;
                case KEY_R:
                    break;
                case KEY_L: 
                    break;
                case KEY_B:
                    audio_play(SOUND_GENERIC_CLINK);
                    return;
                case KEY_DOWN:
                    ; //empty statement for C compiler
                    u8 task_id = task_add((TaskCallback)(task_scroll), 0);
                    tasks[task_id].priv[0] = 0; //bgid
                    tasks[task_id].priv[1] = 100; //delta
                    tasks[task_id].priv[2] = 1; //mode
                    break;
                case KEY_UP:
	                bgid_mod_y_offset(0, 5, 2);
                    break;
                case KEY_LEFT:
	                bgid_mod_x_offset(0, 5, 1);
                    break;
                case KEY_RIGHT:
	                bgid_mod_x_offset(0, 5, 2);
                    break;
            };
        }
        break;
	}
	
	
}

void task_scroll(u8 task_id){
    struct Task curr_task = tasks[task_id];
    u8 bgid = curr_task.priv[0];
    u8 delta = curr_task.priv[1];
    u8 mode = curr_task.priv[2];
    bgid_mod_y_offset(bgid, delta, mode);
}

#endif