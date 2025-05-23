#include <lcom/lcf.h>

#include "controller/ev/ev.h"

int(handle_timer_event)(Game *game, uint32_t counter) {
  if (game == NULL) {
    fprintf(stderr, "handle_kbd_event: game pointer cannot be null.");
    return 1;
  }

  switch (game->state) {
    case START:
      /* code */
      break;

    case PAUSE:
      /* code */
      break;

    case GAME:
      if (counter % 60 == 0) {
        for (uint8_t i = 0; i < game->num_enemies; i++) {
          if (game->enemies[i].active) {
            move_enemy(&game->enemies[i], game);
          }
        }
      }
      break;

    case EXIT:
      /* code */
      break;

    default:
      fprintf(stderr, "handle_timer_event: invalid game state.");
      return 1;
  }

  return 0;
}

int(handle_kbd_event)(Game *game, uint8_t scancode) {
  if (game == NULL) {
    fprintf(stderr, "handle_kbd_event: game pointer cannot be null.");
    return 1;
  }

  switch (game->state) {
    case START:
      switch (scancode) {
        case 0x1C: // ENTER
          game->state = GAME;
          break;
        case 0x01: // ESC
          game->state = EXIT;
        default:
          break;
      }
      break;

    case PAUSE:
      switch (scancode) {
        case 0x1C: // ENTER
          game->state = GAME;
          break;
        case 0x01: // ESC
          game->state = START;
        default:
          break;
      }
      break;

    case GAME:
      switch (scancode) {
        case 0x1C: // ENTER
          game->state = PAUSE;
          break;
        case 0x48: // UP
          move_player(&game->player, game, 0, -1);
          break;
        case 0x4D: // RIGHT
          move_player(&game->player, game, 1, 0);
          break;
        case 0x50: // DOWN
          move_player(&game->player, game, 0, 1);
          break;
        case 0x4B: // LEFT
          move_player(&game->player, game, -1, 0);
          break;
        case 0x39: // SPACE
          drop_bomb(game, 2, 3);
          break;
        default:
          break;
      }
      break;

    case EXIT:
      break;

    default:
      fprintf(stderr, "handle_kbd_event: invalid game state.");
      return 1;
  }

  return 0;
}

int(handle_mouse_event)(Game *game, mouse_info_t mouse_info) {
  if (game == NULL) {
    fprintf(stderr, "handle_mouse_event: game pointer cannot be null.");
    return 1;
  }

  switch (game->state) {
    case START:
      break;

    case PAUSE:
      break;

    case GAME:
      if (mouse_info.rb)
        printf("%d : %u -> right button\n", mouse_info.x, mouse_info.y);
      else if (mouse_info.lb)
        printf("%d : %u -> left button\n", mouse_info.x, mouse_info.y);
      break;

    case EXIT:
      break;

    default:
      fprintf(stderr, "handle_mouse_event: invalid game state.");
      return 1;
  }

  return 0;
}
