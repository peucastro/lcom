#include <lcom/lcf.h>

#include "view/view.h"

static uint8_t *frame_cache = NULL;
static uint8_t cache_initialized = 0;
static uint8_t last_menu_option = 255;
static game_state_t last_game_state = EXIT;
static uint8_t last_level = 0;
static int last_score = 0;
static uint8_t game_background_cached = 0;

void(cleanup_cache)(void) {
  if (frame_cache != NULL) {
    free(frame_cache);
    frame_cache = NULL;
    cache_initialized = 0;
    last_menu_option = 255;
    last_game_state = EXIT;
    last_level = 0;
    last_score = 0;
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

int(draw_pause_menu)(Game *game) {
  const Resources *res = get_resources();
  if (res == NULL) {
    fprintf(stderr, "draw_pause_menu: resources not loaded");
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

    Sprite *menu_img = res->menu_pause[idx];
    if (menu_img == NULL) {
      fprintf(stderr, "draw_pause_menu: menu_pause[%u] is NULL", idx);
      return 1;
    }

    if (draw_sprite(menu_img, 0, 0) != 0) {
      fprintf(stderr, "draw_pause_menu: failed to blit menu sprite");
      return 1;
    }

    if (cache_current_frame() != 0) {
      fprintf(stderr, "draw_pause_menu: failed to cache menu.");
      return 1;
    }
    last_menu_option = game->menu_option;
  }
  else {
    if (restore_cached_frame() != 0) {
      fprintf(stderr, "draw_pause_menu: failed to restore cached menu.");
      return 1;
    }
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

  int px, py;
  for (uint8_t i = 0; i < game->num_enemies; i++) {
    Entity *e = &game->enemies[i];
    if (!e->active)
      continue;
    px = (int) round(e->move.px);
    py = (int) round(e->move.py) + cell_size;
    if (draw_sprite(e->sprite, px, py) != 0) {
      fprintf(stderr, "draw_next_frame: failed to draw enemy %d", i);
      return 1;
    }
  }

  Entity *powerup = &game->powerup;
  if (powerup->active) {
    if (draw_sprite(powerup->sprite, powerup->x * cell_size, cell_size + powerup->y * cell_size) != 0) {
      fprintf(stderr, "draw_dynamic_entities: failed to draw powerup sprite.");
      return 1;
    }
  }

  if (game->player.active) {
    int ppx = (int) round(game->player.move.px);
    int ppy = (int) round(game->player.move.py) + cell_size;
    if (draw_sprite(game->player.sprite, ppx, ppy) != 0) {
      fprintf(stderr, "draw_dynamic_entities: failed to draw player sprite.");
      return 1;
    }
  }

  for (uint8_t i = 0; i < game->num_explosions; i++) {
    Entity *expl = &game->explosions[i];
    if (expl->active) {
      if (draw_sprite(expl->sprite, expl->x * cell_size, cell_size + expl->y * cell_size) != 0) {
        fprintf(stderr, "draw_dynamic_entities: failed to draw explosion sprite at index %d.", i);
        return 1;
      }
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

    if (game->door.active) {
      if (draw_sprite(game->door.sprite, game->door.x * cell_size, cell_size + game->door.y * cell_size) != 0) {
        fprintf(stderr, "draw_background_cache: failed to draw door sprite.");
        return 1;
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

/**
 * @brief Draws the "SCORE:" label using rectangles
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_score_label)(void) {
  if (draw_sprite(get_resources()->score_sprite, 0, 0) != 0) {
    fprintf(stderr, "draw_score_label: failed to draw score sprite.");
    return 1;
  }

  return 0;
}

/**
 * @brief Draws a single digit using seven-segment display style
 *
 * Renders a digit (0-9) using seven segments made of rectangles.
 *
 * @param x X-coordinate for the digit position
 * @param y Y-coordinate for the digit position
 * @param digit The digit to draw (0-9)
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_digit)(uint16_t x, uint16_t y, int digit) {
  const uint16_t segment_width = 20;
  const uint16_t segment_height = 4;
  const uint32_t color = 0x000000;

  const uint8_t segments[10][7] = {
    {1, 1, 1, 0, 1, 1, 1}, // 0
    {0, 0, 1, 0, 0, 1, 0}, // 1
    {1, 0, 1, 1, 1, 0, 1}, // 2
    {1, 0, 1, 1, 0, 1, 1}, // 3
    {0, 1, 1, 1, 0, 1, 0}, // 4
    {1, 1, 0, 1, 0, 1, 1}, // 5
    {1, 1, 0, 1, 1, 1, 1}, // 6
    {1, 0, 1, 0, 0, 1, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
  };

  if (digit < 0 || digit > 9) {
    return 1;
  }

  if (segments[digit][0]) // top
    graphics_draw_rectangle(x, y, segment_width, segment_height, color);
  if (segments[digit][3]) // middle
    graphics_draw_rectangle(x, y + segment_width, segment_width, segment_height, color);
  if (segments[digit][6]) // bottom
    graphics_draw_rectangle(x, y + segment_width * 2, segment_width, segment_height, color);

  if (segments[digit][1]) // top left
    graphics_draw_rectangle(x, y, segment_height, segment_width, color);
  if (segments[digit][2]) // top right
    graphics_draw_rectangle(x + segment_width - segment_height, y, segment_height, segment_width, color);
  if (segments[digit][4]) // bottom left
    graphics_draw_rectangle(x, y + segment_width + segment_height, segment_height, segment_width, color);
  if (segments[digit][5]) // bottom right
    graphics_draw_rectangle(x + segment_width - segment_height, y + segment_width + segment_height, segment_height, segment_width, color);

  return 0;
}

/**
 * @brief Draws the player's current score
 *
 * Renders the score label and numeric value to the screen.
 *
 * @param game Pointer to the game instance
 * @param x X-coordinate for the score display
 * @param y Y-coordinate for the score display
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_score)(Game *game, uint16_t x, uint16_t y) {
  if (game == NULL) {
    return 1;
  }

  int score = game->score;
  const uint16_t digit_spacing = 30;

  if (draw_score_label() != 0) {
    fprintf(stderr, "draw_score: failed to draw score label.");
    return 1;
  }
  x += 180;

  int temp = (score < 0) ? -score : score;
  int num_digits = 1;
  int div = 10;

  while (temp / div > 0) {
    num_digits++;
    div *= 10;
  }

  x += 8;

  div = 1;
  for (int i = 0; i < num_digits; i++) {
    div *= 10;
  }

  while (div > 1) {
    div /= 10;
    int digit = (score / div) % 10;
    if (digit < 0)
      digit = -digit;
    if (draw_digit(x, y, digit) != 0) {
      fprintf(stderr, "draw_score: failed to draw digit at position (%u, %u).", x, y);
      return 1;
    }
    x += digit_spacing;
  }

  return 0;
}

/**
 * @brief Draws the player's remaining lives as heart sprites
 *
 * Renders heart sprites representing the player's current life count.
 *
 * @param game Pointer to the game instance
 * @param x X-coordinate for the lives display
 * @param y Y-coordinate for the lives display
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_lives)(Game *game, uint16_t x, uint16_t y) {
  if (game == NULL) {
    fprintf(stderr, "draw_lives: game pointer is NULL.\n");
    return 1;
  }

  int lives = game->player.data;
  const uint16_t start_x = 448;

  uint16_t x_pos = start_x;
  Sprite *heart = get_resources()->heart_sprite;

  if (heart != NULL) {
    for (uint8_t i = 0; i < lives; i++) {
      draw_sprite(heart, x_pos, 0);
      x_pos += heart->width;
    }
  }

  return 0;
}

/**
 * @brief Draws the top score bar containing score and lives
 *
 * Renders the gray background bar and calls functions to draw
 * the score and lives information.
 *
 * @param game Pointer to the game instance
 *
 * @return 0 upon success, non-zero otherwise
 */
static int(draw_score_bar)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "draw_score_bar: game pointer cannot be null.");
    return 1;
  }

  if (graphics_draw_rectangle(0, 0, vbe_get_h_res(), 64, 0xB0B0B0) != 0) {
    fprintf(stderr, "draw_score_bar: failed to draw score background.");
    return 1;
  }

  if (draw_score(game, 20, 10) != 0) {
    fprintf(stderr, "draw_score_bar: failed to draw score.");
    return 1;
  }

  if (draw_lives(game, 10, 10) != 0) {
    fprintf(stderr, "draw_score_bar: failed to draw lives.\n");
    return 1;
  }

  last_score = game->score;
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

  if (draw_score_bar(game) != 0) {
    fprintf(stderr, "draw_game: failed to draw score bar.");
    return 1;
  }

  return 0;
}

int(draw_win_screen)(void) {
  if (!cache_initialized) {
    if (draw_sprite(get_resources()->win_sprite, 0, 0) != 0) {
      fprintf(stderr, "draw_win_menu: failed to draw win screen.");
      return 1;
    }

    if (cache_current_frame() != 0) {
      fprintf(stderr, "draw_start_menu: failed to cache menu.");
      return 1;
    }
  }
  else {
    if (restore_cached_frame() != 0) {
      fprintf(stderr, "draw_win_screen: failed to restore cached menu.");
      return 1;
    }
  }

  return 0;
}

int(draw_lose_screen)(void) {
  if (!cache_initialized) {
    if (draw_sprite(get_resources()->lose_sprite, 0, 0) != 0) {
      fprintf(stderr, "draw_lose_menu: failed to draw lose screen.");
      return 1;
    }

    if (cache_current_frame() != 0) {
      fprintf(stderr, "draw_start_menu: failed to cache menu.");
      return 1;
    }
  }
  else {
    if (restore_cached_frame() != 0) {
      fprintf(stderr, "draw_lose_screen: failed to restore cached menu.");
      return 1;
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

  if (game->state != last_game_state) {
    cache_initialized = 0;
    game_background_cached = 0;
    last_menu_option = 3;
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
      if (draw_pause_menu(game) != 0) {
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

    case WIN:
      if (draw_win_screen() != 0) {
        fprintf(stderr, "draw_next_frame: failed to win screen.");
        return;
      }
      break;

    case LOSE:
      if (draw_lose_screen() != 0) {
        fprintf(stderr, "draw_next_frame: failed to lose screen.");
        return;
      }
      break;

    case EXIT:
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
