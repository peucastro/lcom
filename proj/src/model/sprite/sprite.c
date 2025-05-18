#include <lcom/lcf.h>

#include "model/sprite/sprite.h"

Sprite *(create_sprite) (const char *pic[]) {
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL) {
    fprintf(stderr, "create_sprite: failed to allocate memory for sprite.");
    return NULL;
  }

  xpm_image_t img;
  sp->map = (char *) xpm_load(pic, XPM_8_8_8_8, &img);
  if (sp->map == NULL) {
    fprintf(stderr, "create_sprite: failed to load XPM image.");
    free(sp);
    return NULL;
  }

  sp->width = img.width;
  sp->height = img.height;
  return sp;
}

void(destroy_sprite)(Sprite *sp) {
  if (sp == NULL) {
    return;
  }
  if (sp->map) {
    free(sp->map);
  }
  free(sp);
  sp = NULL;
}

int(draw_sprite)(Sprite *sp, int16_t x, int16_t y) {
  if (sp == NULL) {
    fprintf(stderr, "draw_sprite: sp pointer cannot be null.");
    return 1;
  }
  if (x >= vbe_get_h_res() || y >= vbe_get_v_res()) {
    fprintf(stderr, "draw_sprite: invalid coordinates.");
    return 1;
  }
  if (x + sp->width > vbe_get_h_res() || y + sp->height > vbe_get_v_res()) {
    fprintf(stderr, "draw_sprite: invalid dimensions.");
    return 1;
  }

  uint32_t color;
  for (uint16_t row = 0; row < sp->height; row++) {
    for (uint16_t col = 0; col < sp->width; col++) {
      color = *((uint32_t *) (&sp->map[(row * sp->width + col) * 4]));

      if (color == xpm_transparency_color(XPM_8_8_8_8)) {
        continue;
      }

      if (graphics_draw_pixel(x + col, y + row, color) != 0) {
        fprintf(stderr, "draw_sprite: failed to draw pixel.");
        return 1;
      }
    }
  }

  return 0;
}
