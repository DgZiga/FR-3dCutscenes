#ifndef ASSETS_H
#define ASSETS_H

#include <pokeagb/types.h>

struct asset {
    const u32 tiles_len;
    const u8 *tiles;
    const u32 map_len;
    const u16 *map;
    const u32 pals_len;
    const u8 *pals;
};

#define ASSET(bgid, framestart) \
(struct asset){ \
    .tiles_len = bg##bgid##_##framestart##TilesLen, \
    .tiles = bg##bgid##_##framestart##Tiles, \
    .map_len = bg##bgid##_##framestart##MapLen, \
    .map = bg##bgid##_##framestart##Map, \
    .pals_len = bg##bgid##_##framestart##PalLen, \
    .pals = bg##bgid##_##framestart##Pal, \
}

#endif