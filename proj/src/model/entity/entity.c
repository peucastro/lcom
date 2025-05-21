#include <lcom/lcf.h>

#include "model/entity/entity.h"

int(init_entity)(Entity *entity, int16_t x, int16_t y, Sprite *sp) {
  if (entity == NULL) {
    fprintf(stderr, "init_entity: entity pointer cannot be null.");
    return 1;
  }

  if (sp == NULL) {
    fprintf(stderr, "init_entity: sprite pointer cannot be null.");
    return 1;
  }

  entity->x = x;
  entity->y = y;
  entity->sprite = sp;
  entity->active = true;

  return 0;
}

int(reset_entity)(Entity *entity) {
  if (entity == NULL) {
    fprintf(stderr, "reset_entity: entity pointer cannot be null.");
    return 1;
  }

  entity->x = 0;
  entity->y = 0;
  entity->sprite = NULL;
  entity->active = false;

  return 0;
}
