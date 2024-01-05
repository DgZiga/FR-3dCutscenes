#ifndef SETUP_H
#define SETUP_H

#include <pokeagb/pokeagb.h>
#define CPUFSCPY 0
#define CPUFSSET 1
#define CPUModeFS(size, mode) ((size >> 2) | (mode << 24))


void vblank_cb_spq();
void setup();
void CpuFastSet(void* src, void* dst, u32 mode);

#endif