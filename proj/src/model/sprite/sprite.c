#include <lcom/lcf.h>

#include "model/sprite/sprite.h"

Sprite *(create_sprite) (const char *pic[], int16_t x, int16_t y) {
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  xpm_image_t img;
  if (sp == NULL) {
    return NULL;
  }

  sp->map = (char *) xpm_load(pic, XPM_INDEXED, &img);
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
