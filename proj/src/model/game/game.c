#include <lcom/lcf.h>

#include "model/game/game.h"

void(init_game)(Game *game) {
  game->state = START;
}

void(draw_game)(Game *game) {
  if (game->state == START) {
    graphics_draw_rectangle(0, 0, 1152, 864, 0xFF0000);
  }
  else if (game->state == GAME) {
    graphics_draw_rectangle(0, 0, 1152, 864, 0x0000FF);
  }
}
