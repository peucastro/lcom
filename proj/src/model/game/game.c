#include <lcom/lcf.h>

#include "model/game/game.h"

int(init_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "init_game: game pointer cannot be null.");
    return 1;
  }

  game->state = START;

  game->board = create_board_from_file("/home/lcom/labs/proj/src/assets/boards/level1.txt");
  if (game->board == NULL) {
    fprintf(stderr, "init_game: failed to load game board.");
    return 1;
  }

  const Resources *resources = get_resources();

  uint8_t n_enemies = 0, n_bricks = 0, n_walls = 0, n_bombs = 0;
  for (uint8_t r = 0; r < game->board->height; r++) {
    for (uint8_t c = 0; c < game->board->width; c++) {
      switch (game->board->elements[r][c]) {
        case ENEMY: n_enemies++; break;
        case BRICK: n_bricks++; break;
        case WALL: n_walls++; break;
        case BOMB: n_bombs++; break;
        default: break;
      }
    }
  }

  game->num_enemies = n_enemies;
  game->num_bricks = n_bricks;
  game->num_walls = n_walls;
  game->num_bombs = n_bombs;

  game->enemies = malloc(n_enemies * sizeof(Entity *));
  game->bricks = malloc(n_bricks * sizeof(Entity *));
  game->walls = malloc(n_walls * sizeof(Entity *));
  game->player = NULL;

  if ((n_enemies && !game->enemies) ||
      (n_bricks && !game->bricks) ||
      (n_walls && !game->walls)) {
    fprintf(stderr, "init_game: failed to allocate entity arrays.");
    free(game->enemies);
    game->enemies = NULL;

    free(game->bricks);
    game->bricks = NULL;

    free(game->walls);
    game->walls = NULL;

    destroy_board(game->board);
    return 1;
  }

  uint8_t ei = 0, bri = 0, wi = 0, boi = 0;
  for (uint8_t r = 0; r < game->board->height; r++) {
    for (uint8_t c = 0; c < game->board->width; c++) {
      BoardElement el = game->board->elements[r][c];
      switch (el) {
        case PLAYER:
          game->player = create_entity(c, r, resources->player_sprite);
          break;
        case ENEMY:
          game->enemies[ei++] = create_entity(c, r, resources->enemy_sprite);
          break;
        case BRICK:
          game->bricks[bri++] = create_entity(c, r, resources->brick_sprite);
          break;
        case WALL:
          game->walls[wi++] = create_entity(c, r, resources->wall_sprite);
          break;
        case BOMB:
          game->bombs[boi++] = create_entity(c, r, resources->bomb_sprite);
          break;
        default:
          // nothing to do for empty tiles
          break;
      }
    }
  }

  return 0;
}

void(destroy_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "destroy_game: game pointer cannot be null.");
    return;
  }

  if (game->player) {
    destroy_entity(game->player);
    game->player = NULL;
  }

  if (game->enemies) {
    for (uint8_t i = 0; i < game->num_enemies; i++) {
      if (game->enemies[i] != NULL) {
        destroy_entity(game->enemies[i]);
      }
    }
    free(game->enemies);
    game->enemies = NULL;
  }

  if (game->bricks) {
    for (uint8_t i = 0; i < game->num_bricks; i++) {
      if (game->bricks[i] != NULL) {
        destroy_entity(game->bricks[i]);
      }
    }
    free(game->bricks);
    game->bricks = NULL;
  }

  if (game->walls) {
    for (uint8_t i = 0; i < game->num_walls; i++) {
      if (game->walls[i] != NULL) {
        destroy_entity(game->walls[i]);
      }
    }
    free(game->walls);
    game->walls = NULL;
  }

  destroy_board(game->board);
  game->board = NULL;
}
