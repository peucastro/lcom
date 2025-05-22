#include <lcom/lcf.h>

#include "controller/ev/ev.h"

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
        {
          PlayerMove moveUp = {0, -1};
          update_player(&game->player.base, &game->board, &moveUp);
          break;
        }
        case 0x4D: // RIGHT
        {
          PlayerMove moveRight = {1, 0};
          update_player(&game->player.base, &game->board, &moveRight);
          break;
        }
        case 0x50: // DOWN
        {
          PlayerMove moveDown = {0, 1};
          update_player(&game->player.base, &game->board, &moveDown);
          break;
        }
        case 0x4B: // LEFT
        {
          PlayerMove moveLeft = {-1, 0};
          update_player(&game->player.base, &game->board, &moveLeft);
          break;
        }
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
