#ifndef ASSETS_H
#define ASSETS_H

#include <pokeagb/types.h>
#include "built_graphics/bg0_0.h"
#include "built_graphics/bg1_0.h"
#include "built_graphics/bg2_0.h"
#include "built_graphics/bg2_60.h"
#include "built_graphics/bg3_0.h"

struct asset {
    const u32 tiles_len;
    const u8 *tiles;
    const u32 map_len;
    const u16 *map;
    const u32 pals_len;
    const u8 *pals;
};

#define ASSET_NAME(bgid, framestart) bg##bgid##_##framestart##_asset

#define ASSET(bgid, framestart) \
struct asset bg##bgid##_##framestart##_asset ={ \
    .tiles_len = bg##bgid##_##framestart##TilesLen, \
    .tiles = bg##bgid##_##framestart##Tiles, \
    .map_len = bg##bgid##_##framestart##MapLen, \
    .map = bg##bgid##_##framestart##Map, \
    .pals_len = bg##bgid##_##framestart##PalLen, \
    .pals = bg##bgid##_##framestart##Pal, \
};


#define ASSET2(bgid, framestart) \
(struct asset){ \
    .tiles_len = bg##bgid##_##framestart##TilesLen, \
    .tiles = bg##bgid##_##framestart##Tiles, \
    .map_len = bg##bgid##_##framestart##MapLen, \
    .map = bg##bgid##_##framestart##Map, \
    .pals_len = bg##bgid##_##framestart##PalLen, \
    .pals = bg##bgid##_##framestart##Pal, \
}

#endif