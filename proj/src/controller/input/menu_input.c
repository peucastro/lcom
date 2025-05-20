#include "controller/input/menu_input.h"

//static int current_option = 0;
void handle_menu_input(Game *game, Key k) {
  /*
  switch (k) {
    case KEY_UP: current_option = (current_option + MAX_OPTS - 1) % MAX_OPTS; break;
    case KEY_DOWN: current_option = (current_option + 1) % MAX_OPTS; break;
    case KEY_ENTER:
      if (current_option == 0) game->state = START;
      else if (current_option == 1) game->state = EXIT;
      break;
    default: break;
  }
  */

    switch (k) {
        case KEY_ENTER: game->state = GAME; break;
        case KEY_ESCAPE: game->state = EXIT; break;
        default: break;
    }
    
}

