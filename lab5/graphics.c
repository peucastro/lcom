#include <lcom/lcf.h>

#include "graphics.h"

static vbe_mode_info_t mode_info;
static uint8_t *video_mem;
static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel;
static uint16_t bytes_per_pixel;
static uint8_t vram_size;

int(set_video_mode)(uint16_t mode) {
  struct reg86 args;
  memset(&args, 0, sizeof(args));

  args.ah = VBE_FUNCTION;
  args.al = VBE_SET_MODE;
  args.bx = mode | VBE_LINEAR_MODE;
  args.intno = VBE_INT;

  if (sys_int86(&args) != 0) {
    perror("set_video_mode: failed to call sys_int86.");
    return 1;
  }

  if (args.ah != VBE_CALL_SUCCESS || args.al != VBE_FUNCTION) {
    if (args.ah == VBE_CALL_FAIL) {
      perror("set_video_mode: vbe call fail.");
    }
    if (args.ah == VBE_CALL_NOT_SUPPORTED) {
      perror("set_video_mode: vbe call not supported.");
    }
    if (args.ah == VBE_CALL_INVALID) {
      perror("set_video_mode: vbe call invalid.");
    }
    return 1;
  }

  return 0;
}

int(map_graphics_vram)(uint16_t mode) {
  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    perror("map_graphics_vram: failed to get mode info.");
    return 1;
  }

  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;
  vram_size = h_res * v_res * bytes_per_pixel;

  struct minix_mem_range mr;
  memset(&mr, 0, sizeof(mr));
  mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
    perror("map_graphics_vram: failed to request permission.");
    return 1;
  }

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED) {
    perror("map_graphics_vram: map failed");
    return 1;
  }

  return 0;
}

int(draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x > h_res || y > v_res) {
    perror("draw_pixel: invalid coordinate.");
    return 1;
  }

  uint8_t *pixel = video_mem + mode_info.BytesPerScanLine * y + bytes_per_pixel * x;
  memcpy(pixel, &color, bytes_per_pixel);

  return 0;
}

int(draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (x > h_res || y > v_res || width > h_res || height > v_res) {
    perror("draw_rectangle: invalid dimensions.");
    return 1;
  }

  for (uint16_t i = x; i < x + width; i++) {
    for (uint16_t j = y; j < y + height; j++) {
      if (draw_pixel(i, j, color) != 0) {
        perror("draw_rectangle: failed to paint pixel.");
        return 1;
      }
    }
  }

  return 0;
}

uint16_t(get_h_res)(void) {
  return h_res;
}

uint16_t(get_v_res)(void) {
  return v_res;
}

uint8_t(get_bits_per_pixel)(void) {
  return bits_per_pixel;
}

uint16_t(get_bytes_per_pixel)(void) {
  return bytes_per_pixel;
}
