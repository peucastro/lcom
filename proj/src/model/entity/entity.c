#include <lcom/lcf.h>

#include "model/entity/entity.h"

Entity *(create_entity) (int16_t x, int16_t y, Sprite *sp) {
  if (sp == NULL) {
    fprintf(stderr, "create_entity: sprite pointer cannot be null.");
    return NULL;
  }

  Entity *entity = malloc(sizeof(Entity));
  if (entity == NULL) {
    fprintf(stderr, "create_entity: failed to allocate memory for entity.");
    return NULL;
  }

  entity->x = x;
  entity->y = y;
  entity->sprite = sp;
  entity->alive = true;

  return entity;
}

void(destroy_entity)(Entity *entity) {
  if (entity == NULL) {
    fprintf(stderr, "destroy_entity: entity pointer cannot be null.");
    return;
  }

  free(entity);
}
