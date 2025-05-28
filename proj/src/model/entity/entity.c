#include <lcom/lcf.h>

#include "model/entity/entity.h"
#include "model/game/game.h"

#define CELL_SIZE 64

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

  e->move.px = x * CELL_SIZE;
  e->move.py = y * CELL_SIZE;

  e->move.tx = x;
  e->move.ty = y;
  e->move.sx = x;
  e->move.sy = y;

  e->anim = NULL;

  e->move.moving = false;
  e->move.tick = 0;
  e->move.total_ticks = 60; 

  e->update = default_update;

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

  if (e->anim) { destroy_animSprite(e->anim); e->anim = NULL; }

  return 0;
}

void default_update(Entity *e, Game *game, uint32_t counter) {

  if (!e->active) return;

  if (e->move.moving) {
    float step_x = (e->move.tx - e->x) * ((float)CELL_SIZE / e->move.total_ticks);
    float step_y = (e->move.ty - e->y) * ((float)CELL_SIZE / e->move.total_ticks);

    e->move.px += step_x;
    e->move.py += step_y;

    if (++e->move.tick >= e->move.total_ticks) {
      game->board.elements[e->move.sy][e->move.sx] = EMPTY_SPACE;
      e->x = e->move.tx;
      e->y = e->move.ty;
      e->move.px = e->x * CELL_SIZE;
      e->move.py = e->y * CELL_SIZE;
      e->move.moving = false;
      e->move.tick   = 0;
    }
  }

  if (e->move.moving && e->anim) {
    update_animSprite(e->anim);
    e->sprite = e->anim->sp;
  }
}
