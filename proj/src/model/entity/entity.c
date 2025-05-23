#include <lcom/lcf.h>

#include "model/entity/entity.h"

int(init_entity)(Entity *e, int16_t x, int16_t y, Sprite *sp, int16_t data) {
  if (e == NULL) {
    fprintf(stderr, "init_entity: entity pointer cannot be null.");
    return 1;
  }

  if (sp == NULL) {
    fprintf(stderr, "init_entity: sprite pointer cannot be null.");
    return 1;
  }

  e->x = x;
  e->y = y;
  e->dir = RIGHT;
  e->sprite = sp;
  e->active = true;
  e->data = data;

  return 0;
}

int(reset_entity)(Entity *e) {
  if (e == NULL) {
    fprintf(stderr, "reset_entity: entity pointer cannot be null.");
    return 1;
  }

  e->x = 0;
  e->y = 0;
  e->sprite = NULL;
  e->data = 0;
  e->active = false;

  return 0;
}
