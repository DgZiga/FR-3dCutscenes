#ifndef ANIMATION_C
#define ANIMATION_C

#include "animation_bg_show.h"

u32 calc_char_start(struct BgConfig *bg_cfg){
    return ADDR_VRAM + (bg_cfg->character_base*0x4000);
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

void load_asset_to_bg(struct drawing_keyframe drawing_keyframe){
    u8 pal_id = drawing_keyframe.bgid; //just convenient
    struct BgConfig *bg_cfg = get_bg_config_by_bg_id(drawing_keyframe.bgid);
    void *buffer = malloc(0x1000);
    gpu_pal_apply_compressed((void *)(drawing_keyframe.asset.pals), pal_id * 16, 32);
    LZ77UnCompWram((void *)(drawing_keyframe.asset.map), (void *)buffer);
    lz77UnCompVram((void *)(drawing_keyframe.asset.tiles), (void *)calc_char_start(bg_cfg));
    bgid_set_tilemap(drawing_keyframe.bgid, buffer);
    bgid_mark_for_sync(drawing_keyframe.bgid);
    gpu_sync_bg_show(drawing_keyframe.bgid);
}
#endif