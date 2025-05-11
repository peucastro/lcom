#include <lcom/lcf.h>

#include "model/game/game.h"

static game_state gs = START;

game_state(get_game_state)(void) {
  return gs;
}

void(update_game_state)(game_state new_state) {
  gs = new_state;
}
