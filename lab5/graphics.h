#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>

#include "vbe.h"

int(set_video_mode)(uint16_t mode);

int(map_graphics_vram)(uint16_t mode);

int(draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

uint16_t(get_h_res)(void);

uint16_t(get_v_res)(void);

uint8_t(get_bits_per_pixel)(void);

#endif
