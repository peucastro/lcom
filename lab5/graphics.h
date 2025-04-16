#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>

#include "vbe.h"

int(graphics_set_video_mode)(uint16_t mode);

int(graphics_map_vram)(uint16_t mode);

int(graphics_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int(graphics_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int(graphics_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

#endif
