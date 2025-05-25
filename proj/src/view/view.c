#include <lcom/lcf.h>

#include "view/view.h"

static uint8_t *background_cache = NULL;
static uint8_t background_cache_initialized = 0;

void(cleanup_background_cache)(void) {
  if (background_cache != NULL) {
    free(background_cache);
    background_cache = NULL;
    background_cache_initialized = 0;
  }
}

/**
 * @brief Draws and caches the static background elements
 *
 * Creates a cached version of the background containing walls and empty spaces
 * filled with the specified background color. The cache is initialized once
 * and reused for subsequent frames to improve rendering performance.
 *
 * @param game Pointer to the game instance containing wall entities
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_background_cache)(Game *game) {
  const uint8_t cell_size = 64;

  if (background_cache == NULL) {
    size_t cache_size = vbe_get_h_res() * vbe_get_v_res() * vbe_get_bytes_per_pixel();
    background_cache = (uint8_t *) malloc(cache_size);
    if (background_cache == NULL) {
      fprintf(stderr, "draw_background_cache: failed to allocate background cache.");
      return 1;
    }
  }

  if (!background_cache_initialized) {
    vbe_set_video_mem(background_cache);

    if (graphics_draw_rectangle(0, 64, vbe_get_h_res(), vbe_get_v_res() - 64, 0x388700) != 0) {
      vbe_set_video_mem(NULL);
      fprintf(stderr, "draw_background_cache: failed to draw green background.");
      return 1;
    }

    for (uint8_t i = 0; i < game->num_walls; i++) {
      Entity *wall = &game->walls[i];
      if (wall->active) {
        if (draw_sprite(wall->sprite, wall->x * cell_size, cell_size + wall->y * cell_size) != 0) {
          vbe_set_video_mem(NULL);
          fprintf(stderr, "draw_background_cache: failed to draw wall sprite at index %d.", i);
          return 1;
        }
      }
    }

    vbe_set_video_mem(NULL);
    background_cache_initialized = 1;
  }

  size_t cache_size = vbe_get_h_res() * vbe_get_v_res() * vbe_get_bytes_per_pixel();
  memcpy(vbe_get_video_mem(), background_cache, cache_size);

  return 0;
}

int(draw_start_menu)(Game *game) {
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

/**
 * @brief Draws all dynamic entities to the screen
 *
 * Renders moving and changing game elements including bricks, bombs, enemies,
 * and the player. These entities are drawn on top of the cached background
 * in the proper Z-order to ensure correct visual layering.
 *
 * @param game Pointer to the game instance containing all entity arrays
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_dynamic_entities)(Game *game) {
  const uint8_t cell_size = 64;

  for (uint8_t i = 0; i < game->num_bricks; i++) {
    Entity *brick = &game->bricks[i];
    if (brick->active) {
      if (draw_sprite(brick->sprite, brick->x * cell_size, cell_size + brick->y * cell_size) != 0) {
        fprintf(stderr, "draw_dynamic_entities: failed to draw brick sprite at index %d.", i);
        return 1;
      }
    }
  }

  for (uint8_t i = 0; i < game->num_bombs; i++) {
    Entity *bomb = &game->bombs[i];
    if (bomb->active) {
      if (draw_sprite(bomb->sprite, bomb->x * cell_size, cell_size + bomb->y * cell_size) != 0) {
        fprintf(stderr, "draw_dynamic_entities: failed to draw bomb sprite at index %d.", i);
        return 1;
      }
    }
  }

  for (uint8_t i = 0; i < game->num_enemies; i++) {
    Entity *enemy = &game->enemies[i];
    if (enemy->active) {
      if (draw_sprite(enemy->sprite, enemy->x * cell_size, cell_size + enemy->y * cell_size) != 0) {
        fprintf(stderr, "draw_dynamic_entities: failed to draw enemy sprite at index %d.", i);
        return 1;
      }
    }
  }

  if (game->player.active) {
    if (draw_sprite(game->player.sprite, game->player.x * cell_size, cell_size + game->player.y * cell_size) != 0) {
      fprintf(stderr, "draw_dynamic_entities: failed to draw player sprite.");
      return 1;
    }
  }

  return 0;
}

int(draw_game)(Game *game) {
  if (draw_background_cache(game) != 0) {
    fprintf(stderr, "draw_game: failed to draw background cache.");
    return 1;
  }

  if (draw_dynamic_entities(game) != 0) {
    fprintf(stderr, "draw_game: failed to draw dynamic entities.");
    return 1;
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

  switch (game->state) {
    case START:
      if (draw_start_menu(game) != 0) {
        fprintf(stderr, "draw_next_frame: failed to draw start menu.");
        return;
      }
      break;

    case PAUSE:
      if (graphics_clear_screen() != 0) {
        fprintf(stderr, "draw_next_frame: failed to clear screen.");
        return;
      }
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
