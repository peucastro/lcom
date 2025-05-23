#include "controller/input/game_input.h"

void handle_game_input(Game *game, Key k) {
  switch (k) {
    case KEY_LEFT: update_player(&game->player, game, -1, 0); break;
    case KEY_RIGHT: update_player(&game->player, game, 1, 0); break;
    case KEY_UP: update_player(&game->player, game, 0, -1); break;
    case KEY_DOWN: update_player(&game->player, game, 0, 1); break;
    case KEY_ESCAPE: game->state = START; break;
    default: break;
  }
}

