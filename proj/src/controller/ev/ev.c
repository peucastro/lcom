#include <lcom/lcf.h>

#include "controller/ev/ev.h"

void(handle_kbd_event)(Game *game, uint8_t scancode) {
  switch (game->state) {
    case START:
      switch (scancode) {
        case 0x1C: // ENTER
          game->state = GAME;
          break;
        default:
          break;
      }
      break;

    case GAME:
      switch (scancode) {
        case 0x1C: // ENTER
          game->state = START;
          break;
        case 0x48: // UP arrow
          game->player->y -= 1;
          break;
        case 0x4D: // RIGHT arrow
          game->player->x += 1;
          break;
        case 0x50: // DOWN arrow
          game->player->y += 1;
          break;
        case 0x4B: // LEFT arrow
          game->player->x -= 1;
          break;
        default:
          break;
      }
      break;

    default:
      break;
  }
}
