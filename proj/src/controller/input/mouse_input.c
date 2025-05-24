#include "controller/input/mouse_input.h"

void handle_mouse_input(Game *game) {
    switch (game->state) {
        case START:
        case PAUSE:
            handle_menu_mouse(game);
            break;
        case GAME:
            break;
        default:
            break;
    }
}


void handle_menu_mouse(Game *game) {
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
        if (game->menu_option == 1) game->state = GAME;
        else if (game->menu_option == 2) game->state = EXIT;
    }

    prev_lb = lb;
}
