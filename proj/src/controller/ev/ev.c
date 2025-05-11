#include <lcom/lcf.h>

#include "controller/ev/ev.h"

void(handle_event)(Game *game, uint8_t scancode) {
  if (game->state == START && scancode == 0x1C) {
    game->state = GAME;
  }
}
