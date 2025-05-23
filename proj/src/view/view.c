#include <lcom/lcf.h>

#include "view/view.h"

int draw_start_menu(Game *game) {
  if (!game) {
    fprintf(stderr, "draw_start_menu: game pointer is NULL\n");
    return 1;
  }

  const Resources *res = get_resources();
  if (!res) {
    fprintf(stderr, "draw_start_menu: resources not loaded\n");
    return 1;
  }

  /* clamp menu_option to [0,2] just in case */
  uint8_t idx = game->menu_option;
  if (idx > 2) idx = 0;

  Sprite *menu_img = res->menu_sprite[idx];
  if (!menu_img) {
    fprintf(stderr, "draw_start_menu: menu_sprite[%u] is NULL\n", idx);
    return 1;
  }

  if (draw_sprite(menu_img, 0, 0) != 0) {
    fprintf(stderr, "draw_start_menu: failed to blit menu sprite\n");
    return 1;
  }

  return 0;
}

int(draw_pause_menu)(void) {
  if (graphics_draw_rectangle(0, 0, 1024, 768, 0x0000FF) != 0) {
    fprintf(stderr, "draw_pause_menu: failed to draw pause menu background.");
    return 1;
  }

  return 0;
}

int(draw_game)(Game *game) {
  const uint8_t cell_size = 64;

  if (graphics_draw_rectangle(0, 0, 1024, cell_size, 0xB0B0B0) != 0) {
    fprintf(stderr, "draw_game: failed to draw top bar.");
    return 1;
  }

  if (game->player.active) {
    if (draw_sprite(game->player.sprite, game->player.x * cell_size, cell_size + game->player.y * cell_size) != 0) {
      fprintf(stderr, "draw_game: failed to draw player sprite.");
      return 1;
    }
  }

  for (uint8_t i = 0; i < game->num_enemies; i++) {
    Entity *enemy = &game->enemies[i];
    if (enemy->active) {
      if (draw_sprite(enemy->sprite, enemy->x * cell_size, cell_size + enemy->y * cell_size) != 0) {
        fprintf(stderr, "draw_game: failed to draw enemy sprite at index %d.", i);
        return 1;
      }
    }
  }

  for (uint8_t i = 0; i < game->num_bricks; i++) {
    Entity *brick = &game->bricks[i];
    if (brick->active) {
      if (draw_sprite(brick->sprite, brick->x * cell_size, cell_size + brick->y * cell_size) != 0) {
        fprintf(stderr, "draw_game: failed to draw brick sprite at index %d.", i);
        return 1;
      }
    }
  }

  for (uint8_t i = 0; i < game->num_walls; i++) {
    Entity *wall = &game->walls[i];
    if (wall->active) {
      if (draw_sprite(wall->sprite, wall->x * cell_size, cell_size + wall->y * cell_size) != 0) {
        fprintf(stderr, "draw_game: failed to draw wall sprite at index %d.", i);
        return 1;
      }
    }
  }

  for (uint8_t i = 0; i < game->num_bombs; i++) {
    Entity *bomb = &game->bombs[i];
    if (bomb->active) {
      if (draw_sprite(bomb->sprite, bomb->x * cell_size, cell_size + bomb->y * cell_size) != 0) {
        fprintf(stderr, "draw_game: failed to draw bomb sprite at index %d.", i);
        return 1;
      }
    }
  }

  return 0;
}

int(draw_mouse)(mouse_info_t mouse_info) {
  Sprite *mouse_sprite = get_resources()->handpointing_sprite;
  if (mouse_info.rb || mouse_info.lb) {
    mouse_sprite = get_resources()->handopen_sprite;
  }
  if (draw_sprite(mouse_sprite, mouse_info.x, mouse_info.y) != 0) {
    fprintf(stderr, "draw_mouse: failed to draw cursor sprite.");
    return 1;
  }

  return 0;
}

void(draw_next_frame)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "draw_next_frame: game pointer cannot be null.");
    return;
  }

  if (graphics_clear_screen() != 0) {
    fprintf(stderr, "draw_next_frame: failed to clear screen.");
    return;
  }

  switch (game->state) {
    case START:
      if (draw_start_menu(game) != 0) {
        fprintf(stderr, "draw_next_frame: failed to draw start menu.");
        return;
      }
      break;

    case PAUSE:
      if (draw_pause_menu() != 0) {
        fprintf(stderr, "draw_next_frame: failed to draw pause menu.");
        return;
      }
      break;

    case GAME:
      if (draw_game(game) != 0) {
        fprintf(stderr, "draw_next_frame: failed to draw game.");
        return;
      }
      break;

    default:
      fprintf(stderr, "draw_next_frame: invalid game state.");
      return;
  }

  if (draw_mouse(mouse_get_info()) != 0) {
    fprintf(stderr, "draw_next_frame: failed to draw cursor.");
    return;
  }
}
