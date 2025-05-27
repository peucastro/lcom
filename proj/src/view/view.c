#include <lcom/lcf.h>

#include "view/view.h"

static uint8_t *frame_cache = NULL;
static uint8_t cache_initialized = 0;
static uint8_t last_menu_option = 255;
static game_state_t last_game_state = EXIT;
static uint8_t last_level = 0;
static uint8_t game_background_cached = 0;

void(cleanup_cache)(void) {
  if (frame_cache != NULL) {
    free(frame_cache);
    frame_cache = NULL;
    cache_initialized = 0;
    last_menu_option = 255;
    last_game_state = EXIT;
    last_level = 0;
    game_background_cached = 0;
  }
}

/**
 * @brief Caches the current frame buffer to memory
 *
 * Allocates memory for the frame cache if needed and copies the current video memory to the cache.
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(cache_current_frame)(void) {
  if (frame_cache == NULL) {
    size_t cache_size = vbe_get_h_res() * vbe_get_v_res() * vbe_get_bytes_per_pixel();
    frame_cache = (uint8_t *) malloc(cache_size);
    if (frame_cache == NULL) {
      fprintf(stderr, "cache_current_frame: failed to allocate frame cache.");
      return 1;
    }
  }

  size_t cache_size = vbe_get_h_res() * vbe_get_v_res() * vbe_get_bytes_per_pixel();
  memcpy(frame_cache, vbe_get_video_mem(), cache_size);
  cache_initialized = 1;
  return 0;
}

/**
 * @brief Restores the cached frame buffer to video memory
 *
 * Copies the cached frame buffer back to video memory if the cache is initialized.
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(restore_cached_frame)(void) {
  if (frame_cache == NULL || !cache_initialized) {
    return 1;
  }

  size_t cache_size = vbe_get_h_res() * vbe_get_v_res() * vbe_get_bytes_per_pixel();
  memcpy(vbe_get_video_mem(), frame_cache, cache_size);
  return 0;
}

int(draw_start_menu)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "draw_start_menu: game pointer is NULL");
    return 1;
  }

  if (!cache_initialized || game->menu_option != last_menu_option) {
    const Resources *res = get_resources();
    if (res == NULL) {
      fprintf(stderr, "draw_start_menu: resources not loaded");
      return 1;
    }

    /* clamp menu_option to [0,2] just in case */
    uint8_t idx = game->menu_option;
    if (idx > 2)
      idx = 0;

    Sprite *menu_img = res->menu_sprite[idx];
    if (menu_img == NULL) {
      fprintf(stderr, "draw_start_menu: menu_sprite[%u] is NULL", idx);
      return 1;
    }

    if (draw_sprite(menu_img, 0, 0) != 0) {
      fprintf(stderr, "draw_start_menu: failed to blit menu sprite");
      return 1;
    }

    if (cache_current_frame() != 0) {
      fprintf(stderr, "draw_start_menu: failed to cache menu.");
      return 1;
    }
    last_menu_option = game->menu_option;
  }
  else {
    if (restore_cached_frame() != 0) {
      fprintf(stderr, "draw_start_menu: failed to restore cached menu.");
      return 1;
    }
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
 * @brief Draws all dynamic entities (bricks, bombs, enemies, player) to the screen
 *
 * Draws all moving and changing game elements on top of the cached background.
 *
 * @param game Pointer to the game instance
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_dynamic_entities)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "draw_dynamic_entities: game pointer cannot be null.");
    return 1;
  }

  const uint8_t cell_size = 64;

  for (uint8_t i = 0; i < game->num_enemies; i++) {
    Entity *enemy = &game->enemies[i];
    if (enemy->active) {
      if (draw_sprite(enemy->sprite, enemy->x * cell_size, cell_size + enemy->y * cell_size) != 0) {
        fprintf(stderr, "draw_dynamic_entities: failed to draw enemy sprite at index %d.", i);
        return 1;
      }
    }
  }

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

  for (uint8_t i = 0; i < game->num_powerups; i++) {
    Entity *powerup = &game->powerups[i];
    if (powerup->active) {
      if (draw_sprite(powerup->sprite, powerup->x * cell_size, cell_size + powerup->y * cell_size) != 0) {
        fprintf(stderr, "draw_dynamic_entities: failed to draw powerup sprite at index %d.", i);
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

/**
 * @brief Draws and caches the static background (walls, ground) for the game
 *
 * Draws the static background and walls, then caches the result for fast redraws.
 *
 * @param game Pointer to the game instance
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_background_cache)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "draw_background_cache: game pointer cannot be null.");
    return 1;
  }
  const uint8_t cell_size = 64;

  if (!game_background_cached) {
    if (graphics_draw_rectangle(0, 64, vbe_get_h_res(), vbe_get_v_res() - 64, 0x388700) != 0) {
      fprintf(stderr, "draw_background_cache: failed to draw green background.");
      return 1;
    }

    for (uint8_t i = 0; i < game->num_walls; i++) {
      Entity *wall = &game->walls[i];
      if (wall->active) {
        if (draw_sprite(wall->sprite, wall->x * cell_size, cell_size + wall->y * cell_size) != 0) {
          fprintf(stderr, "draw_background_cache: failed to draw wall sprite at index %d.", i);
          return 1;
        }
      }
    }

    if (cache_current_frame() != 0) {
      fprintf(stderr, "draw_background_cache: failed to cache background.");
      return 1;
    }
    game_background_cached = 1;
  }
  else {
    if (restore_cached_frame() != 0) {
      fprintf(stderr, "draw_background_cache: failed to restore cached background.");
      return 1;
    }
  }

  return 0;
}

int(draw_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "draw_game: game pointer cannot be null.");
    return 1;
  }
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

  if (game->state != last_game_state) {
    cache_initialized = 0;
    game_background_cached = 0;
    last_game_state = game->state;
  }

  if (game->state == GAME && game->level != last_level) {
    game_background_cached = 0;
    last_level = game->level;
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
