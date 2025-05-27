#include <lcom/lcf.h>
#include "controller/ev/ev.h"
#include "view/view.h"

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
        update_enemies(game);
        update_bombs(game);
        draw_next_frame(game);
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

int(handle_kbd_event)(Game *game, Key key) {
  if (game == NULL) {
    fprintf(stderr, "handle_kbd_event: game pointer cannot be null.");
    return 1;
  }

  switch (game->state) {
    case START:
    case PAUSE:
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
          game->state = (game->state == START) ? EXIT : START;
          break;
        default:
          break;
      }
      break;

    case GAME:
      switch (key) {
        case KEY_UP:
          move_player(&game->player, game, 0, -1);
          break;
        case KEY_RIGHT:
          move_player(&game->player, game, 1, 0);
          break;
        case KEY_DOWN:
          move_player(&game->player, game, 0, 1);
          break;
        case KEY_LEFT:
          move_player(&game->player, game, -1, 0);
          break;
        case KEY_ENTER:
          game->state = PAUSE;
          break;
        case KEY_SPACE:
          drop_bomb(game);
          break;
        case KEY_ESCAPE:
          game->state = START;
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
    case PAUSE:
      // handle menu button highlighting
      if (mouse_info.x >= START_BX && mouse_info.x < START_BX + BUTTON_W &&
          mouse_info.y >= START_BY && mouse_info.y < START_BY + BUTTON_H) {
        game->menu_option = 1;
      }
      else if (mouse_info.x >= EXIT_BX && mouse_info.x < EXIT_BX + BUTTON_W &&
               mouse_info.y >= EXIT_BY && mouse_info.y < EXIT_BY + BUTTON_H) {
        game->menu_option = 2;
      }
      else {
        game->menu_option = 0;
      }

      // handle clicks
      if (mouse_info.lb) {
        if (game->menu_option == 1) {
          game->state = GAME;
        }
        else if (game->menu_option == 2) {
          game->state = EXIT;
        }
      }
      break;

    case GAME:
      if (mouse_info.rb)
        printf("%d : %d -> right click\n", mouse_info.x, mouse_info.y);
      else if (mouse_info.lb)
        printf("%d : %d -> left click\n", mouse_info.x, mouse_info.y);
      break;

    case EXIT:
      break;

    default:
      fprintf(stderr, "handle_mouse_event: invalid game state.");
      return 1;
  }

  return 0;
}
