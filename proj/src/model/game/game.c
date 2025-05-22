#include <lcom/lcf.h>

#include "model/game/game.h"

int(init_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "init_game: game pointer cannot be null.");
    return 1;
  }

  game->state = START;
  game->num_enemies = 0;
  game->num_bricks = 0;
  game->num_walls = 0;
  game->num_bombs = 0;

  if (load_board(&game->board, "/home/lcom/labs/proj/src/assets/boards/level1.txt") != 0) {
    fprintf(stderr, "init_game: failed to load game board.");
    return 1;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "init_game: failed to load resources.");
    reset_board(&game->board);
    return 1;
  }

  for (uint8_t r = 0; r < game->board.height; r++) {
    for (uint8_t c = 0; c < game->board.width; c++) {
      switch (game->board.elements[r][c]) {
        case ENEMY:
          if (game->num_enemies < MAX_ENEMIES)
            game->num_enemies++;
          break;
        case BRICK:
          if (game->num_bricks < MAX_BRICKS)
            game->num_bricks++;
          break;
        case WALL:
          if (game->num_walls < MAX_WALLS)
            game->num_walls++;
          break;
        case BOMB:
          if (game->num_bombs < MAX_BOMBS)
            game->num_bombs++;
          break;
        default: break;
      }
    }
  }

  uint8_t ei = 0, bri = 0, wi = 0, boi = 0;
  for (uint8_t r = 0; r < game->board.height; r++) {
    for (uint8_t c = 0; c < game->board.width; c++) {
      board_element_t el = game->board.elements[r][c];
      switch (el) {
        case PLAYER:
          if (init_player(&game->player, c, r, resources->player_down_sprite) != 0) {
            fprintf(stderr, "init_game: failed to initialize player entity.");
            return 1;
          }
          game->player.base.active = true;
          break;
        case ENEMY:
          if (ei < MAX_ENEMIES) {
            if (init_entity(&game->enemies[ei], c, r, resources->enemy_sprite) != 0) {
              fprintf(stderr, "init_game: failed to initialize enemy entity at index %d.", ei);
              return 1;
            }
            ei++;
          }
          break;
        case BRICK:
          if (bri < MAX_BRICKS) {
            if (init_entity(&game->bricks[bri], c, r, resources->brick_sprite) != 0) {
              fprintf(stderr, "init_game: failed to initialize brick entity at index %d.", bri);
              return 1;
            }
            bri++;
          }
          break;
        case WALL:
          if (wi < MAX_WALLS) {
            if (init_entity(&game->walls[wi], c, r, resources->wall_sprite) != 0) {
              fprintf(stderr, "init_game: failed to initialize wall entity at index %d.", wi);
              return 1;
            }
            wi++;
          }
          break;
        case BOMB:
          if (boi < MAX_BOMBS) {
            if (init_entity(&game->bombs[boi], c, r, resources->bomb_sprite) != 0) {
              fprintf(stderr, "init_game: failed to initialize bomb entity at index %d.", boi);
              return 1;
            }
            boi++;
          }
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

  for (uint8_t i = 0; i < game->num_enemies; i++) {
    reset_entity(&game->enemies[i]);
  }
  game->num_enemies = 0;

  for (uint8_t i = 0; i < game->num_bricks; i++) {
    reset_entity(&game->bricks[i]);
  }
  game->num_bricks = 0;

  for (uint8_t i = 0; i < game->num_walls; i++) {
    reset_entity(&game->walls[i]);
  }
  game->num_walls = 0;

  for (uint8_t i = 0; i < game->num_bombs; i++) {
    reset_entity(&game->bombs[i]);
  }
  game->num_bombs = 0;

  if (reset_board(&game->board) != 0) {
    fprintf(stderr, "destroy_game: failed to reset game board.");
  }

  return 0;
}

int(move_player)(Game *game, int16_t xmov, int16_t ymov) {
  if (!game || !game->player.base.active) {
    fprintf(stderr, "move_player: game not initialized or player not active.");
    return 1;
  }

  int16_t new_x = game->player.base.x + xmov;
  int16_t new_y = game->player.base.y + ymov;

  if (new_x < 0 || new_x >= game->board.width ||
      new_y < 0 || new_y >= game->board.height) {
    fprintf(stderr, "move_player: invalid coordinate.");
    return 1;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "move_player: failed to load resources.");
    return 1;
  }

  if (xmov > 0) {
    game->player.base.sprite = resources->player_right_sprite;
  }
  else if (xmov < 0) {
    game->player.base.sprite = resources->player_left_sprite;
  }
  else if (ymov > 0) {
    game->player.base.sprite = resources->player_down_sprite;
  }
  else if (ymov < 0) {
    game->player.base.sprite = resources->player_up_sprite;
  }

  board_element_t destination = game->board.elements[new_y][new_x];

  switch (destination) {
    case EMPTY_SPACE:
    case POWERUP:
      game->board.elements[game->player.base.y][game->player.base.x] = EMPTY_SPACE;
      game->board.elements[new_y][new_x] = PLAYER;
      game->player.base.x = new_x;
      game->player.base.y = new_y;
      break;

    case WALL:
    case BRICK:
    case BOMB:
    case ENEMY:
      break;
    default:
      fprintf(stderr, "move_player: invalid destination.");
      return 1;
  }
  return 0;
}
