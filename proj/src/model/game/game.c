#include <lcom/lcf.h>

#include "model/game/game.h"

int(init_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "init_game: game pointer cannot be null.");
    return 1;
  }

  game->state = START;

  game->board = create_board("/home/lcom/labs/proj/src/assets/boards/level1.txt");
  if (game->board == NULL) {
    fprintf(stderr, "init_game: failed to load game board.");
    return 1;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "init_game: failed to load resources.");
    destroy_board(game->board);
    return 1;
  }

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
  game->bombs = malloc(n_bombs * sizeof(Entity *));
  game->player = NULL;

  if ((n_enemies && !game->enemies) ||
      (n_bricks && !game->bricks) ||
      (n_walls && !game->walls) ||
      (n_bombs && !game->bombs)) {
    fprintf(stderr, "init_game: failed to allocate entity arrays.");

    free(game->enemies);
    game->enemies = NULL;

    free(game->bricks);
    game->bricks = NULL;

    free(game->walls);
    game->walls = NULL;

    free(game->bombs);
    game->bombs = NULL;

    if (destroy_board(game->board) != 0) {
      fprintf(stderr, "init_game: failed to destroy game board after error.");
    }
    return 1;
  }

  uint8_t ei = 0, bri = 0, wi = 0, boi = 0;
  for (uint8_t r = 0; r < game->board->height; r++) {
    for (uint8_t c = 0; c < game->board->width; c++) {
      board_element_t el = game->board->elements[r][c];
      switch (el) {
        case PLAYER:
          game->player = create_entity(c, r, resources->player_down_sprite);
          if (game->player == NULL) {
            fprintf(stderr, "init_game: failed to create player entity.");
            destroy_game(game);
            return 1;
          }
          break;
        case ENEMY:
          game->enemies[ei] = create_entity(c, r, resources->enemy_sprite);
          if (game->enemies[ei] == NULL) {
            fprintf(stderr, "init_game: failed to create enemy entity at index %d.", ei);
            destroy_game(game);
            return 1;
          }
          ei++;
          break;
        case BRICK:
          game->bricks[bri] = create_entity(c, r, resources->brick_sprite);
          if (game->bricks[bri] == NULL) {
            fprintf(stderr, "init_game: failed to create brick entity at index %d.", bri);
            destroy_game(game);
            return 1;
          }
          bri++;
          break;
        case WALL:
          game->walls[wi] = create_entity(c, r, resources->wall_sprite);
          if (game->walls[wi] == NULL) {
            fprintf(stderr, "init_game: failed to create wall entity at index %d.", wi);
            destroy_game(game);
            return 1;
          }
          wi++;
          break;
        case BOMB:
          game->bombs[boi] = create_entity(c, r, resources->bomb_sprite);
          if (game->bombs[boi] == NULL) {
            fprintf(stderr, "init_game: failed to create bomb entity at index %d.", boi);
            destroy_game(game);
            return 1;
          }
          boi++;
          break;
        default:
          // nothing to do for empty tiles
          break;
      }
    }
  }

  return 0;
}

int(destroy_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "destroy_game: game pointer cannot be null.");
    return 1;
  }

  if (game->player) {
    if (destroy_entity(game->player) != 0) {
      fprintf(stderr, "destroy_game: failed to destroy player entity.");
    }
    game->player = NULL;
  }

  if (game->enemies) {
    for (uint8_t i = 0; i < game->num_enemies; i++) {
      if (game->enemies[i] != NULL) {
        if (destroy_entity(game->enemies[i]) != 0) {
          fprintf(stderr, "destroy_game: failed to destroy enemy entity at index %d.", i);
        }
      }
    }
    free(game->enemies);
    game->enemies = NULL;
  }

  if (game->bricks) {
    for (uint8_t i = 0; i < game->num_bricks; i++) {
      if (game->bricks[i] != NULL) {
        if (destroy_entity(game->bricks[i]) != 0) {
          fprintf(stderr, "destroy_game: failed to destroy brick entity at index %d.", i);
        }
      }
    }
    free(game->bricks);
    game->bricks = NULL;
  }

  if (game->walls) {
    for (uint8_t i = 0; i < game->num_walls; i++) {
      if (game->walls[i] != NULL) {
        if (destroy_entity(game->walls[i]) != 0) {
          fprintf(stderr, "destroy_game: failed to destroy wall entity at index %d.", i);
        }
      }
    }
    free(game->walls);
    game->walls = NULL;
  }

  if (game->bombs) {
    for (uint8_t i = 0; i < game->num_bombs; i++) {
      if (game->bombs[i] != NULL) {
        if (destroy_entity(game->bombs[i]) != 0) {
          fprintf(stderr, "destroy_game: failed to destroy bomb entity at index %d.", i);
        }
      }
    }
    free(game->bombs);
    game->bombs = NULL;
  }

  if (destroy_board(game->board) != 0) {
    fprintf(stderr, "destroy_game: failed to destroy game board.");
  }
  game->board = NULL;

  return 0;
}

int(move_player)(Game *game, int16_t xmov, int16_t ymov) {
  if (!game || !game->player || !game->board) {
    fprintf(stderr, "move_player: game not initialized.");
    return 1;
  }

  int16_t new_x = game->player->x + xmov;
  int16_t new_y = game->player->y + ymov;

  if (new_x < 0 || new_x >= game->board->width ||
      new_y < 0 || new_y >= game->board->height) {
    fprintf(stderr, "move_player: invalid coordinate.");
    return 1;
  }

  board_element_t destination = game->board->elements[new_y][new_x];

  switch (destination) {
    case EMPTY_SPACE:
    case POWERUP:
      game->board->elements[game->player->y][game->player->x] = EMPTY_SPACE;
      game->board->elements[new_y][new_x] = PLAYER;
      game->player->x = new_x;
      game->player->y = new_y;
      break;

    case WALL: // TODO
      break;
    case BRICK: // TODO
      break;
    case BOMB: // TODO
      break;
    case ENEMY: // TODO
      break;
    default:
      fprintf(stderr, "move_player: invalid destination.");
      return 1;
  }
  return 0;
}
