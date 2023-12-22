#ifndef ASSETS_H
#define ASSETS_H

#include <pokeagb/types.h>
#include "built_graphics/bg0_0.h"
#include "built_graphics/bg1_0.h"
#include "built_graphics/bg2_0.h"
#include "built_graphics/bg3_0.h"

struct asset {
    const u32 tiles_len;
    const u8 *tiles;
    const u32 map_len;
    const u16 *map;
    const u32 pals_len;
    const u8 *pals;
};

#define ASSET(bgid, frameid) \
struct asset bg##bgid##_##frameid##_asset ={ \
    .tiles_len = bg##bgid##_##frameid##TilesLen, \
    .tiles = bg##bgid##_##frameid##Tiles, \
    .map_len = bg##bgid##_##frameid##MapLen, \
    .map = bg##bgid##_##frameid##Map, \
    .pals_len = bg##bgid##_##frameid##PalLen, \
    .pals = bg##bgid##_##frameid##Pal, \
};


#endif