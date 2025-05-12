#include <lcom/lcf.h>

#include "model/sprite/sprite.h"

Sprite *(create_sprite) (const char *pic[], int16_t x, int16_t y) {
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  xpm_image_t img;
  if (sp == NULL) {
    return NULL;
  }

  sp->map = (char *) xpm_load(pic, XPM_8_8_8_8, &img);
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->x = x;
  sp->y = y;
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

int(draw_sprite)(Sprite *sp) {
  if (sp == NULL) {
    fprintf(stderr, "draw_sprite: sp pointer cannot be null.");
    return 1;
  }
  if (sp->x >= vbe_get_mode().XResolution || sp->y >= vbe_get_mode().YResolution) {
    fprintf(stderr, "draw_sprite: invalid coordinates.");
    return 1;
  }
  if (sp->x + sp->width > vbe_get_mode().XResolution || sp->y + sp->height > vbe_get_mode().YResolution) {
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

      if (graphics_draw_pixel(sp->x + col, sp->y + row, color) != 0) {
        fprintf(stderr, "draw_sprite: failed to draw pixel.");
        return 1;
      }
    }
  }

  return 0;
}

int(move_sprite)(Sprite *sp, int16_t xmov, int16_t ymov) {
  if (sp == NULL) {
    fprintf(stderr, "move_sprite: sp pointer cannot be null.");
    return 1;
  }
  if (sp->x + xmov > vbe_get_mode().XResolution || sp->y + ymov > vbe_get_mode().YResolution) {
    fprintf(stderr, "move_sprite: invalid coordinates.");
    return 1;
  }

  sp->x = sp->x + xmov;
  sp->y = sp->y + ymov;
  return 0;
}
