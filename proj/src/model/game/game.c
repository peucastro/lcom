#include <lcom/lcf.h>

#include "model/game/game.h"

extern Sprite *wall_sprite;
extern Sprite *brick_sprite;
extern Sprite *player_sprite;
extern Sprite *enemy_sprite;

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

  int n_enemies = 0, n_bricks = 0, n_walls = 0;
  for (int r = 0; r < game->board->height; r++) {
    for (int c = 0; c < game->board->width; c++) {
      switch (game->board->elements[r][c]) {
        case ENEMY: n_enemies++; break;
        case BRICK: n_bricks++; break;
        case WALL: n_walls++; break;
        default: break;
      }
    }
  }

  game->num_enemies = n_enemies;
  game->num_bricks = n_bricks;
  game->num_walls = n_walls;

  game->enemies = malloc(n_enemies * sizeof(Entity *));
  game->bricks = malloc(n_bricks * sizeof(Entity *));
  game->walls = malloc(n_walls * sizeof(Entity *));
  game->player = NULL;

  if ((n_enemies && !game->enemies) ||
      (n_bricks && !game->bricks) ||
      (n_walls && !game->walls)) {
    fprintf(stderr, "init_game: failed to allocate entity arrays.");
    free(game->enemies);
    free(game->bricks);
    free(game->walls);
    destroy_board(game->board);
    return 1;
  }

  int ei = 0, bi = 0, wi = 0;
  for (int r = 0; r < game->board->height; r++) {
    for (int c = 0; c < game->board->width; c++) {
      BoardElement el = game->board->elements[r][c];
      switch (el) {
        case PLAYER:
          game->player = create_entity(c, r, player_sprite);
          game->board->elements[r][c] = EMPTY_SPACE;
          break;
        case ENEMY:
          game->enemies[ei++] = create_entity(c, r, enemy_sprite);
          game->board->elements[r][c] = EMPTY_SPACE;
          break;
        case BRICK:
          game->bricks[bi++] = create_entity(c, r, brick_sprite);
          game->board->elements[r][c] = EMPTY_SPACE;
          break;
        case WALL:
          game->walls[wi++] = create_entity(c, r, wall_sprite);
          game->board->elements[r][c] = EMPTY_SPACE;
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
    return;
  }

  if (game->player) {
    free(game->player);
  }

  if (game->enemies) {
    for (int i = 0; game->enemies[i]; i++) {
      free(game->enemies[i]);
    }
    free(game->enemies);
  }

  if (game->bricks) {
    for (int i = 0; game->bricks[i]; i++) {
      free(game->bricks[i]);
    }
    free(game->bricks);
  }

  if (game->walls) {
    for (int i = 0; game->walls[i]; i++) {
      free(game->walls[i]);
    }
    free(game->walls);
  }

  destroy_board(game->board);
}

void(draw_game)(Game *game) {
  graphics_clear_screen();

  if (game->state == START) {
    graphics_draw_rectangle(0, 0, 1152, 864, 0xFF0000);
  }
  else if (game->state == GAME) {
    graphics_draw_rectangle(0, 0, 1152, 864, 0x0000FF);

    const int cell_size = 32;

    if (game->board != NULL) {
      for (int row = 0; row < game->board->height; row++) {
        for (int col = 0; col < game->board->width; col++) {
          if (game->board->elements[row][col] == EMPTY_SPACE) {
            // TODO: decide background
          }
        }
      }
    }

    if (game->player != NULL) {
      draw_sprite(game->player->sprite, game->player->x * cell_size, game->player->y * cell_size);
    }

    for (int i = 0; game->enemies != NULL && i < game->num_enemies; i++) {
      Entity *enemy = game->enemies[i];
      draw_sprite(enemy->sprite, enemy->x * cell_size, enemy->y * cell_size);
    }

    for (int i = 0; game->bricks != NULL && i < game->num_bricks; i++) {
      Entity *brick = game->bricks[i];
      draw_sprite(brick->sprite, brick->x * cell_size, brick->y * cell_size);
    }

    for (int i = 0; game->walls != NULL && i < game->num_walls; i++) {
      Entity *wall = game->walls[i];
      draw_sprite(wall->sprite, wall->x * cell_size, wall->y * cell_size);
    }
  }
}
