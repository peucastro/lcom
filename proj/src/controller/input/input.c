#include <lcom/lcf.h>

#include "controller/input/input.h"

Key(translate_scancode)(uint8_t sc) {
  switch (sc) {
    // arrow keys
    case 0x48: return KEY_UP;
    case 0x50: return KEY_DOWN;
    case 0x4B: return KEY_LEFT;
    case 0x4D: return KEY_RIGHT;

    // WASD keys
    case 0x11: return KEY_UP;
    case 0x1F: return KEY_DOWN;
    case 0x1E: return KEY_LEFT;
    case 0x20: return KEY_RIGHT;

    case 0x1C: return KEY_ENTER;
    case 0x01: return KEY_ESCAPE;
    default: return KEY_NONE;
  }
}

static void(handle_menu_keyboard)(Game *game, Key key) {
  switch (key) {
    case KEY_LEFT:
      game->menu_option = 1;
      break;

    case KEY_RIGHT:
      game->menu_option = 2;
      break;

    case KEY_ENTER:
      if (game->menu_option == 1) {
        game->state = GAME;
      }
      else if (game->menu_option == 2) {
        game->state = EXIT;
      }
      break;

    case KEY_ESCAPE:
      game->state = EXIT;
      break;

    default:
      break;
  }
}

static void(handle_game_keyboard)(Game *game, Key key) {
  switch (key) {
    case KEY_LEFT:
      update_player(&game->player, game, -1, 0);
      break;
    case KEY_RIGHT:
      update_player(&game->player, game, 1, 0);
      break;
    case KEY_UP:
      update_player(&game->player, game, 0, -1);
      break;
    case KEY_DOWN:
      update_player(&game->player, game, 0, 1);
      break;
    case KEY_ESCAPE:
      game->state = START;
      break;
    default:
      break;
  }
}

void(handle_keyboard_input)(Game *game, Key key) {
  if (game == NULL) {
    fprintf(stderr, "handle_keyboard_input: game pointer cannot be null.");
    return;
  }

  switch (game->state) {
    case START:
    case PAUSE:
      handle_menu_keyboard(game, key);
      break;

    case GAME:
      handle_game_keyboard(game, key);
      break;

    case EXIT:
      // no input handling needed in exit state
      break;

    default:
      fprintf(stderr, "handle_keyboard_input: invalid game state.");
      break;
  }
}

static void(handle_menu_mouse)(Game *game) {
  mouse_info_t info = mouse_get_info();
  int mx = info.x;
  int my = info.y;
  bool lb = info.lb;
  static bool prev_lb = false;

  if (mx >= START_BX && mx < START_BX + BUTTON_W && my >= START_BY && my < START_BY + BUTTON_H) {
    game->menu_option = 1;
  }
  else if (mx >= EXIT_BX && mx < EXIT_BX + BUTTON_W && my >= EXIT_BY && my < EXIT_BY + BUTTON_H) {
    game->menu_option = 2;
  }
  else {
    game->menu_option = 0;
  }

  if (lb && !prev_lb) {
    if (game->menu_option == 1) {
      game->state = GAME;
    }
    else if (game->menu_option == 2) {
      game->state = EXIT;
    }
  }

  prev_lb = lb;
}

static void(handle_game_mouse)(Game *game) {
  // TODO
  (void) game; // suppress unused parameter warning
}

void(handle_mouse_input)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "handle_mouse_input: game pointer cannot be null.");
    return;
  }

  switch (game->state) {
    case START:
    case PAUSE:
      handle_menu_mouse(game);
      break;

    case GAME:
      handle_game_mouse(game);
      break;

    case EXIT:
      // no input handling needed in exit state
      break;

    default:
      fprintf(stderr, "handle_mouse_input: invalid game state.");
      break;
  }
}
