#include <lcom/lcf.h>

#include "graphics.h"

/* static global variable to store vbe mode information.
 * this structure holds details about the selected video mode */
static vbe_mode_info_t mode_info;
/* static global pointer to the mapped video memory in the process's address space.
 * in minix 3, vram is not directly accessible and needs to be mapped */
static uint8_t *video_mem;
// static global variable to store the horizontal resolution of the current graphics mode in pixels
static uint16_t h_res;
// static global variable to store the vertical resolution of the current graphics mode in pixels
static uint16_t v_res;
// static global variable to store the number of bits per pixel for the current graphics mode
static uint8_t bits_per_pixel;
// static global variable to store the number of bytes per pixel, derived from bits per pixel
static uint16_t bytes_per_pixel;
// static global variable to store the size of the video ram in bytes for the current graphics mode
static uint32_t vram_size;

uint32_t(R)(uint32_t color) {
  /* compute a bitmask with RedMaskSize bits set to 1.
   * BIT(n) is assumed to return 2^n, so BIT(n) - 1 gives a mask of n bits */
  uint32_t mask = BIT(mode_info.RedMaskSize) - 1;

  /* shift the input color to the right by the red field position.
   * then apply the mask to isolate the red component */
  return (color >> mode_info.RedFieldPosition) & mask;
}

uint32_t(G)(uint32_t color) {
  // create a mask with GreenMaskSize bits set to 1
  uint32_t mask = BIT(mode_info.GreenMaskSize) - 1;

  // extract the green component by shifting and masking
  return (color >> mode_info.GreenFieldPosition) & mask;
}

uint32_t(B)(uint32_t color) {
  // create a mask with BlueMaskSize bits set to 1
  uint32_t mask = BIT(mode_info.BlueMaskSize) - 1;

  // extract the blue component by shifting and masking
  return (color >> mode_info.BlueFieldPosition) & mask;
}

int(graphics_set_video_mode)(uint16_t mode) {
  // holds the arguments for the bios interrupt call
  struct reg86 args;
  // clear the reg86 structure to avoid unexpected behavior
  if (memset(&args, 0, sizeof(args)) == NULL) {
    perror("graphics_set_video_mode: failed to clear reg86.");
    return 1;
  }

  // set ah register to VBE_FUNCTION (0x4f) to indicate a vbe function call
  args.ah = VBE_FUNCTION;
  // set al register to VBE_SET_MODE (0x02) to select the "set vbe mode" function
  args.al = VBE_SET_MODE;
  /* set bx register to the desired video mode number ored with vbe_linear_mode.
   * setting bit 14 of bx enables the linear frame buffer model for easier vram access */
  args.bx = mode | VBE_LINEAR_MODE;
  // set intno to VBE_INT (0x10) to specify the bios video services interrupt
  args.intno = VBE_INT;

  // call sys_int86 to invoke the bios interrupt with the specified register values
  if (sys_int86(&args) != 0) {
    perror("graphics_set_video_mode: failed to call sys_int86.");
    return 1;
  }

  /* check the return status in ah and al registers after the vbe call.
   * al should be 0x4f if the vbe function is supported, and ah should be 0x00 for success */
  if (args.ah != VBE_CALL_SUCCESS || args.al != VBE_FUNCTION) {
    // check if ah indicates a failed function call (0x01).
    if (args.ah == VBE_CALL_FAIL) {
      perror("graphics_set_video_mode: vbe call fail.");
    }
    // check if ah indicates the function is not supported in the current hardware configuration (0x02)
    else if (args.ah == VBE_CALL_NOT_SUPPORTED) {
      perror("graphics_set_video_mode: vbe call not supported.");
    }
    // check if ah indicates the function is invalid in the current video mode (0x03)
    else if (args.ah == VBE_CALL_INVALID) {
      perror("graphics_set_video_mode: vbe call invalid.");
    }
    // handle other non-success return codes in ah
    else {
      perror("graphics_set_video_mode: function not supported.");
    }
    return 1;
  }

  return 0;
}

int(graphics_map_vram)(uint16_t mode) {
  /* check if setting the memory of the mode_info structure to zero fails.
   * mode_info will store the video mode information */
  if (memset(&mode_info, 0, sizeof(mode_info)) == NULL) {
    perror("graphics_map_vram: failed to clear mode_info.");
    return 1;
  }
  /* call the vbe_get_mode_info function to retrieve information about the specified video mode.
   * this function is provided by the lcf */
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    perror("graphics_map_vram: failed to get mode info.");
    return 1;
  }

  // initialize the static global variables with the values retrieved from the vbe_get_mode_info call
  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;
  vram_size = h_res * v_res * bytes_per_pixel;

  // declare a struct minix_mem_range to define the physical memory range
  struct minix_mem_range mr;
  // check if setting the memory of the minix_mem_range structure to zero fails
  if (memset(&mr, 0, sizeof(mr)) == NULL) {
    perror("graphics_map_vram: failed to clear minix_mem_range.");
    return 1;
  }
  // set the base physical address of the memory range to the physical base pointer obtained from mode_info.
  mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;
  // set the limit of the memory range to the base address plus the total vram size.
  mr.mr_limit = mr.mr_base + vram_size;

  // request permission to map the specified physical memory range into the process's address space using the sys_privctl kernel call
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
    perror("graphics_map_vram: failed to request permission.");
    return 1;
  }

  // map the physical memory region into the process's virtual address space using the vm_map_phys kernel call.
  video_mem = (uint8_t *) vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  // check if the memory mapping failed.
  if ((void *) video_mem == MAP_FAILED) {
    perror("graphics_map_vram: map failed.");
    return 1;
  }

  return 0;
}

int(graphics_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  /* check if the given pixel coordinates (x, y) are outside the screen boundaries
   * note: coordinates are valid if 0 <= x < h_res and 0 <= y < v_res */
  if (x > h_res || y > v_res) {
    perror("graphics_draw_pixel: invalid coordinate.");
    return 1;
  }

  /* calculate the memory address of the pixel to be drawn
   each row has h_res pixels, so we move y rows down by doing (y * h_res).
   we then move x pixels right within that row.
   then we multiply by bytes_per_pixel to account for the color depth */
  uint8_t *pixel = video_mem + (y * h_res + x) * bytes_per_pixel;

  // write the given color value to the calculated pixel location in memory
  if (memcpy(pixel, &color, bytes_per_pixel) == NULL) {
    perror("graphics_draw_pixel: failed to draw pixel.");
    return 1;
  }

  return 0;
}

int(graphics_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  // check if the starting point or the line length goes beyond the screen width
  if (x > h_res || y > v_res || x + len > h_res) {
    perror("graphics_draw_hline: invalid coordinate.");
    return 1;
  }

  /* draw the horizontal line pixel by pixel.
   * we iterate from the start position x up to (x + len), keeping the same y */
  for (uint16_t col = x; col < x + len; col++) {
    // draw each pixel along the horizontal direction
    if (graphics_draw_pixel(col, y, color) != 0) {
      perror("graphics_draw_hline: failed to draw pixel.");
      return 1;
    }
  }

  return 0;
}

int(graphics_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  /* check if the rectangle’s starting point or size would cause it to exceed screen bounds.
   * ensures both bottom and right edges of the rectangle stay within the screen */
  if (x > h_res || y > v_res || x + width > h_res || y + height > v_res) {
    perror("graphics_draw_rectangle: invalid dimensions.");
    return 1;
  }

  /* draw the rectangle row by row using horizontal lines.
   * for each vertical position from y to (y + height - 1), draw a full horizontal line */
  for (uint16_t row = y; row < y + height; row++) {
    // draw a horizontal line of 'width' pixels starting at (x, row)
    if (graphics_draw_hline(x, row, width, color) != 0) {
      perror("graphics_draw_rectangle: failed to draw line.");
      return 1;
    }
  }

  return 0;
}

int(graphics_draw_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  // calculate the width and height of each rectangle in the grid
  uint16_t width = h_res / no_rectangles;
  uint16_t height = v_res / no_rectangles;
  uint32_t color, r, g, b;

  // extract R, G, B components from the base color in case of direct color mode
  uint32_t r_first = R(first);
  uint32_t g_first = G(first);
  uint32_t b_first = B(first);

  // iterate over all grid positions
  for (uint8_t row = 0; row < no_rectangles; row++) {
    for (uint8_t col = 0; col < no_rectangles; col++) {
      // check if mode is indexed
      if (mode == VBE_MODE_1024x768) {
        // compute the color index for indexed mode
        color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
      }
      else {
        // compute RGB components for direct color mode
        r = (r_first + col * step) % (1 << mode_info.RedMaskSize);
        g = (g_first + row * step) % (1 << mode_info.GreenMaskSize);
        b = (b_first + (col + row) * step) % (1 << mode_info.BlueMaskSize);

        // build the final color by shifting components to their proper positions
        color = (r << mode_info.RedFieldPosition) |
                (g << mode_info.GreenFieldPosition) |
                (b << mode_info.BlueFieldPosition);
      }

      // draw the filled rectangle at the appropriate screen location
      if (graphics_draw_rectangle(width * col, height * row, width, height, color) != 0) {
        perror("graphics_draw_matrix: failed to draw rectangle.");
        return 1;
      }
    }
  }

  return 0;
}

int(graphics_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* check if the given pixel coordinates (x, y) are outside the screen boundaries
   * note: coordinates are valid if 0 <= x < h_res and 0 <= y < v_res */
  if (x > h_res || y > v_res) {
    perror("graphics_draw_xpm: invalid coordinates.");
    return 1;
  }

  // declare an xpm_image_t struct to hold metadata (e.g. type, width, height) of the loaded xpm
  xpm_image_t img;

  /* convert the input XPM into a pixel map using the lcf-provided xpm_load function.
   * we use XPM_INDEXED assuming the color mode is indexed for mode 0x105 */
  uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);

  // check if the image loading failed
  if (map == NULL) {
    perror("graphics_draw_xpm: xpm map address is null.");
    return 1;
  }

  /* check if the full image would go beyond the screen limits when drawn from (x, y).
   * prevents drawing outside of VRAM which would cause memory errors */
  else if (x + img.width > h_res || y + img.height > v_res) {
    perror("graphics_draw_xpm: invalid dimensions.");
    return 1;
  }

  /* iterate over every pixel in the loaded image.
   * each row is drawn top-down from the starting y coordinate, and each column left-right from x */
  for (uint16_t row = 0; row < img.height; row++) {
    for (uint16_t col = 0; col < img.width; col++) {
      // compute the address of the pixel color in the map (row-major order)
      uint32_t color = *(map + (row * img.width + col));

      // draw the pixel at the corresponding position on screen
      if (graphics_draw_pixel(x + col, y + row, color) != 0) {
        perror("graphics_draw_xpm: failed to draw pixel.");
        return 1;
      }
    }
  }

  return 0;
}

int(graphics_clear_xpm)(xpm_image_t img, uint16_t x, uint16_t y) {
  if (x > h_res || y > v_res) {
    perror("graphics_clear_xpm: invalid coordinates.");
    return 1;
  }
  else if (x + img.width > h_res || y + img.height > v_res) {
    perror("graphics_clear_xpm: invalid dimensions.");
    return 1;
  }

  if (graphics_draw_rectangle(x, y, img.width, img.height, 0x00000000) != 0) {
    perror("graphics_clear_xpm: failed to clear the image.");
    return 1;
  }

  return 0;
}
