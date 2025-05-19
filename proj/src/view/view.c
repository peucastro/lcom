#include <lcom/lcf.h>

#include "view/view.h"

int(draw_start_menu)(void) {
  if (graphics_draw_rectangle(0, 0, 1024, 768, 0xFF0000) != 0) {
    fprintf(stderr, "draw_start_menu: failed to draw start menu background.");
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

  if (game->player != NULL) {
    if (draw_sprite(game->player->sprite, game->player->x * cell_size, game->player->y * cell_size) != 0) {
      fprintf(stderr, "draw_game: failed to draw player sprite.");
      return 1;
    }
  }

  if (graphics_draw_rectangle(0, 0, 1024, cell_size, 0xB0B0B0) != 0) {
    fprintf(stderr, "draw_game: failed to draw top bar.");
    return 1;
  }

  for (uint8_t i = 0; game->enemies != NULL && i < game->num_enemies; i++) {
    Entity *enemy = game->enemies[i];
    if (draw_sprite(enemy->sprite, enemy->x * cell_size, cell_size + enemy->y * cell_size) != 0) {
      fprintf(stderr, "draw_game: failed to draw enemy sprite at index %d.", i);
      return 1;
    }
  }

  for (uint8_t i = 0; game->bricks != NULL && i < game->num_bricks; i++) {
    Entity *brick = game->bricks[i];
    if (draw_sprite(brick->sprite, brick->x * cell_size, cell_size + brick->y * cell_size) != 0) {
      fprintf(stderr, "draw_game: failed to draw brick sprite at index %d.", i);
      return 1;
    }
  }

  for (uint8_t i = 0; game->walls != NULL && i < game->num_walls; i++) {
    Entity *wall = game->walls[i];
    if (draw_sprite(wall->sprite, wall->x * cell_size, cell_size + wall->y * cell_size) != 0) {
      fprintf(stderr, "draw_game: failed to draw wall sprite at index %d.", i);
      return 1;
    }
  }

  for (uint8_t i = 0; game->bombs != NULL && i < game->num_bombs; i++) {
    Entity *bomb = game->bombs[i];
    if (draw_sprite(bomb->sprite, bomb->x * cell_size, cell_size + bomb->y * cell_size) != 0) {
      fprintf(stderr, "draw_game: failed to draw bomb sprite at index %d.", i);
      return 1;
    }
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
      if (draw_start_menu() != 0) {
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
}
