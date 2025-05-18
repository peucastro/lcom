#include <lcom/lcf.h>

#include "view/view.h"

void(draw_start_menu)(void) {
  graphics_draw_rectangle(0, 0, 1024, 768, 0xFF0000);
}

void(draw_pause_menu)(void) {
  graphics_draw_rectangle(0, 0, 1024, 768, 0x0000FF);
}

void(draw_game)(Game *game) {
  const uint8_t cell_size = 64;

  // TODO: decide background
  /* if (game->board != NULL) {
    for (uint8_t row = 0; row < game->board->height; row++) {
      for (uint8_t col = 0; col < game->board->width; col++) {
        if (game->board->elements[row][col] == EMPTY_SPACE) {
        }
      }
    }
  } */

  if (game->player != NULL) {
    draw_sprite(game->player->sprite, game->player->x * cell_size, game->player->y * cell_size);
  }

  for (uint8_t i = 0; game->enemies != NULL && i < game->num_enemies; i++) {
    Entity *enemy = game->enemies[i];
    draw_sprite(enemy->sprite, enemy->x * cell_size, enemy->y * cell_size);
  }

  for (uint8_t i = 0; game->bricks != NULL && i < game->num_bricks; i++) {
    Entity *brick = game->bricks[i];
    draw_sprite(brick->sprite, brick->x * cell_size, brick->y * cell_size);
  }

  for (uint8_t i = 0; game->walls != NULL && i < game->num_walls; i++) {
    Entity *wall = game->walls[i];
    draw_sprite(wall->sprite, wall->x * cell_size, wall->y * cell_size);
  }

  for (uint8_t i = 0; game->bombs != NULL && i < game->num_bombs; i++) {
    Entity *bomb = game->bombs[i];
    draw_sprite(bomb->sprite, bomb->x * cell_size, bomb->y * cell_size);
  }
}

void(draw_next_frame)(Game *game) {
  if (graphics_clear_screen() != 0) {
    fprintf(stderr, "draw_next_frame: failed to clear screen.");
    return;
  }

  switch (game->state) {
    case START:
      draw_start_menu();
      break;

    case PAUSE:
      draw_pause_menu();
      break;

    case GAME:
      draw_game(game);

    default:
      break;
  }
}
