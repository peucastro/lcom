#include <lcom/lcf.h>

#include "model/entity/player.h"

int(init_player)(Player *p, int16_t x, int16_t y, Sprite *sp) {
  if (p == NULL) {
    fprintf(stderr, "init_player: player pointer cannot be null.");
    return 1;
  }

  if (sp == NULL) {
    fprintf(stderr, "init_player: sprite pointer cannot be null.");
    return 1;
  }

  if (init_entity(&p->base, x, y, sp) != 0) {
    fprintf(stderr, "init_player: failed to initialize base entity.");
    return 1;
  }

  p->base.on_update = player_update;
  p->lives = 3;
  return 0;
}

int(reset_player)(Player *p) {
  if (p == NULL) {
    fprintf(stderr, "reset_player: player pointer cannot be null.");
    return 1;
  }

  if (reset_entity(&p->base) != 0) {
    fprintf(stderr, "reset_player: failed to reset entity.");
    return 1;
  }

  p->lives = 0;
  return 0;
}

void(player_update)(struct Entity *p) {
  // how to update the player?
}
