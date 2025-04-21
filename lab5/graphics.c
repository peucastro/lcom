#include <lcom/lcf.h>

#include "graphics.h"

static vbe_mode_info_t mode_info;
static uint8_t *video_mem;
static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel;
static uint16_t bytes_per_pixel;
static uint32_t vram_size;

int(graphics_set_video_mode)(uint16_t mode) {
  struct reg86 args;
  if (memset(&args, 0, sizeof(args)) == NULL) {
    perror("graphics_set_video_mode: failed to clear reg86.");
    return 1;
  }

  args.ah = VBE_FUNCTION;
  args.al = VBE_SET_MODE;
  args.bx = mode | VBE_LINEAR_MODE;
  args.intno = VBE_INT;

  if (sys_int86(&args) != 0) {
    perror("graphics_set_video_mode: failed to call sys_int86.");
    return 1;
  }

  if (args.ah != VBE_CALL_SUCCESS || args.al != VBE_FUNCTION) {
    if (args.ah == VBE_CALL_FAIL) {
      perror("graphics_set_video_mode: vbe call fail.");
    }
    else if (args.ah == VBE_CALL_NOT_SUPPORTED) {
      perror("graphics_set_video_mode: vbe call not supported.");
    }
    else if (args.ah == VBE_CALL_INVALID) {
      perror("graphics_set_video_mode: vbe call invalid.");
    }
    else {
      perror("graphics_set_video_mode: function not supported.");
    }
    return 1;
  }

  return 0;
}

int(graphics_map_vram)(uint16_t mode) {
  if (memset(&mode_info, 0, sizeof(mode_info)) == NULL) {
    perror("graphics_map_vram: failed to clear mode_info.");
    return 1;
  }
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    perror("graphics_map_vram: failed to get mode info.");
    return 1;
  }

  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;
  vram_size = h_res * v_res * bytes_per_pixel;

  struct minix_mem_range mr;
  if (memset(&mr, 0, sizeof(mr)) == NULL) {
    perror("graphics_map_vram: failed to clear minix_mem_range.");
    return 1;
  }
  mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
    perror("graphics_map_vram: failed to request permission.");
    return 1;
  }

  video_mem = (uint8_t *) vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED) {
    perror("graphics_map_vram: map failed");
    return 1;
  }

  return 0;
}

int(graphics_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x > h_res || y > v_res) {
    perror("graphics_draw_pixel: invalid coordinate.");
    return 1;
  }

  uint8_t *pixel = video_mem + (y * h_res + x) * bytes_per_pixel;
  if (memcpy(pixel, &color, bytes_per_pixel) == NULL) {
    perror("graphics_draw_pixel: failed to draw pixel.");
    return 1;
  }

  return 0;
}

int(graphics_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x > h_res || y > v_res || x + len > h_res) {
    perror("graphics_draw_hline: invalid coordinate.");
    return 1;
  }

  for (uint16_t i = x; i < x + len; i++) {
    if (graphics_draw_pixel(i, y, color) != 0) {
      perror("graphics_draw_hline: failed to draw pixel.");
      return 1;
    }
  }

  return 0;
}

int(graphics_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (x > h_res || y > v_res || x + width > h_res || y + height > v_res) {
    perror("graphics_draw_rectangle: invalid dimensions.");
    return 1;
  }

  for (uint16_t j = y; j < y + height; j++) {
    if (graphics_draw_hline(x, j, width, color) != 0) {
      perror("graphics_draw_rectangle: failed to draw line.");
      return 1;
    }
  }

  return 0;
}

int(graphics_draw_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint16_t width = h_res / no_rectangles;
  uint16_t height = v_res / no_rectangles;
  uint32_t color;

  for (uint8_t row = 0; row < no_rectangles; row++) {
    for (uint8_t col = 0; col < no_rectangles; col++) {
      if (mode == VBE_MODE_1024x768) {
        color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
      }
      else {
        // TODO
      }

      if (graphics_draw_rectangle(width * col, height * row, width, height, color) != 0) {
        perror("graphics_draw_matrix: failed to draw rectangle");
        return 1;
      }
    }
  }

  return 0;
}
