#include <lcom/lcf.h>

#include "model/game/game.h"

int(load_next_level)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "load_next_level: game pointer cannot be null.");
    return 1;
  }

  uint8_t lives = game->player.data;
  game->level++;
  char board_path[128];
  snprintf(board_path, sizeof(board_path), "/home/lcom/labs/proj/src/assets/boards/level%u.txt", game->level);

  if (load_board(&game->board, board_path) != 0) {
    fprintf(stderr, "load_next_level: failed to load board %s\n.", board_path);
    return 1;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "load_next_level: failed to load resources.");
    return 1;
  }

  game->num_enemies = 0;
  game->num_bricks = 0;
  game->num_walls = 0;
  game->num_bombs = 0;

  uint8_t ei = 0, bri = 0, wi = 0, boi = 0, pi = 0;
  for (uint8_t r = 0; r < game->board.height; r++) {
    for (uint8_t c = 0; c < game->board.width; c++) {
      board_element_t el = game->board.elements[r][c];
      switch (el) {
        case PLAYER:
          if (init_entity(&game->player, c, r, resources->player_down_sprite, lives) != 0) {
            fprintf(stderr, "load_next_level: failed to initialize player entity.");
            return 1;
          }
          game->player.active = true;
          break;
        case ENEMY:
          if (ei < MAX_ENEMIES) {
            if (init_entity(&game->enemies[ei], c, r, resources->enemy_left_sprite, 2) != 0) {
              fprintf(stderr, "init_game: failed to initialize enemy entity at index %d.", ei);
              return 1;
            }
            ei++;
          }
          break;
        case BRICK:
          if (bri < MAX_BRICKS) {
            if (init_entity(&game->bricks[bri], c, r, resources->brick_sprites[0], 3) != 0) {
              fprintf(stderr, "init_game: failed to initialize brick entity at index %d.", bri);
              return 1;
            }
            bri++;
          }
          break;
        case WALL:
          if (wi < MAX_WALLS) {
            if (init_entity(&game->walls[wi], c, r, resources->wall_sprite, 0) != 0) {
              fprintf(stderr, "init_game: failed to initialize wall entity at index %d.", wi);
              return 1;
            }
            wi++;
          }
          break;
        case BOMB:
          if (boi < MAX_BOMBS) {
            if (init_entity(&game->bombs[boi], c, r, resources->bomb_sprites[0], 3) != 0) {
              fprintf(stderr, "init_game: failed to initialize bomb entity at index %d.", boi);
              return 1;
            }
            boi++;
          }
          break;

        case POWERUP:
          if (pi < 1) {
            if (init_entity(&game->powerup, c, r, resources->powerup_sprite, 0) != 0) {
              fprintf(stderr, "init_game: failed to initialize powerup entity.");
              return 1;
            }
          }
          pi++;
          break;

        case EMPTY_SPACE:
          // nothing to do for empty tiles
          break;

        default:
          fprintf(stderr, "load_next_level: invalid board element.");
          return 1;
      }
    }
  }
  game->num_enemies = ei;
  game->num_bricks = bri;
  game->num_walls = wi;
  game->num_bombs = boi;

  return 0;
}

int(init_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "init_game: game pointer cannot be null.");
    return 1;
  }

  game->state = START;
  game->menu_option = 0;
  game->level = 0;

  game->num_enemies = 0;
  game->num_bricks = 0;
  game->num_walls = 0;
  game->num_bombs = 0;

  game->player.data = 3;

  if (load_next_level(game) != 0) {
    fprintf(stderr, "init_game: failed to load next level.");
    return 1;
  }

  return 0;
}

int(reset_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "reset_game: game pointer cannot be null.");
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

  reset_entity(&game->powerup);
  game->num_bombs = 0;

  if (reset_board(&game->board) != 0) {
    fprintf(stderr, "reset_game: failed to reset game board.");
  }

  return 0;
}

void(move_player)(Entity *p, Game *game, int16_t xmov, int16_t ymov) {
  if (p == NULL || game == NULL) {
    fprintf(stderr, "move_player: invalid player or game pointer.");
    return;
  }

  if (!p->active) {
    fprintf(stderr, "move_player: player not active.");
    return;
  }

  int16_t new_x = p->x + xmov;
  int16_t new_y = p->y + ymov;

  if (new_x < 0 || new_x >= game->board.width ||
      new_y < 0 || new_y >= game->board.height) {
    fprintf(stderr, "move_player: invalid coordinate.");
    return;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "move_player: failed to load resources.");
    return;
  }

  if (xmov > 0) {
    p->sprite = resources->player_right_sprite;
    p->dir = RIGHT;
  }
  else if (xmov < 0) {
    p->sprite = resources->player_left_sprite;
    p->dir = LEFT;
  }
  else if (ymov > 0) {
    p->sprite = resources->player_down_sprite;
    p->dir = DOWN;
  }
  else if (ymov < 0) {
    p->sprite = resources->player_up_sprite;
    p->dir = UP;
  }

  board_element_t destination = game->board.elements[new_y][new_x];

  switch (destination) {
    case EMPTY_SPACE:
      game->board.elements[p->y][p->x] = EMPTY_SPACE;
      game->board.elements[new_y][new_x] = PLAYER;
      p->x = new_x;
      p->y = new_y;
      break;

    case POWERUP:
      if (game->powerup.active) {
        game->powerup.active = false;

        game->board.elements[p->y][p->x] = EMPTY_SPACE;
        game->board.elements[new_y][new_x] = PLAYER;
        game->player.data++;
        p->x = new_x;
        p->y = new_y;
      }
      break;

    case WALL:
    case BRICK:
    case BOMB:
    case ENEMY:
      // block movement
      break;
    default:
      fprintf(stderr, "move_player: invalid destination.");
      return;
  }
}

void(move_enemy)(Entity *e, Game *game) {
  if (e == NULL || game == NULL) {
    fprintf(stderr, "move_enemy: invalid enemy or game pointer.");
    return;
  }

  if (!e->active) {
    fprintf(stderr, "move_enemy: enemy not active.");
    return;
  }

  const int8_t dx[4] = {0, 1, 0, -1};
  const int8_t dy[4] = {-1, 0, 1, 0};

  int valid_directions[4];
  int valid_count = 0;

  for (int i = 0; i < 4; i++) {
    int16_t new_x = e->x + dx[i];
    int16_t new_y = e->y + dy[i];

    if (new_x < 0 || new_x >= game->board.width ||
        new_y < 0 || new_y >= game->board.height) {
      continue;
    }

    board_element_t destination = game->board.elements[new_y][new_x];
    if (destination == EMPTY_SPACE || destination == POWERUP || destination == PLAYER) {
      valid_directions[valid_count++] = i;
    }
  }

  if (valid_count == 0) {
    return;
  }

  int chosen_dir = valid_directions[rand() % valid_count];
  int16_t new_x = e->x + dx[chosen_dir];
  int16_t new_y = e->y + dy[chosen_dir];

  if (new_x > e->x) {
    e->sprite = get_resources()->enemy_right_sprite;
  }
  else if (new_x < e->x) {
    e->sprite = get_resources()->enemy_left_sprite;
  }

  game->board.elements[e->y][e->x] = EMPTY_SPACE;
  game->board.elements[new_y][new_x] = ENEMY;
  e->x = new_x;
  e->y = new_y;
}

void(update_enemies)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "update_enemies: game pointer cannot be null.");
    return;
  }

  uint8_t active_enemies = 0;
  for (uint8_t i = 0; i < game->num_enemies; i++) {
    if (game->enemies[i].active) {
      move_enemy(&game->enemies[i], game);

      if (i != active_enemies) {
        game->enemies[active_enemies] = game->enemies[i];
      }
      active_enemies++;
    }
  }

  game->num_enemies = active_enemies;
}

void(drop_bomb)(Game *game, int16_t x, int16_t y) {
  if (game == NULL) {
    fprintf(stderr, "drop_bomb: game pointer cannot be null.");
    return;
  }

  if (x < 0 || x >= game->board.width || y < 0 || y >= game->board.height) {
    fprintf(stderr, "drop_bomb: coordinates (%d, %d) are out of bounds.", x, y);
    return;
  }

  if (!game->player.active) {
    fprintf(stderr, "drop_bomb: player is not active.");
    return;
  }

  int16_t dx = abs(x - game->player.x);
  int16_t dy = abs(y - game->player.y);

  if (game->num_bombs >= MAX_BOMBS || game->board.elements[y][x] != EMPTY_SPACE || dx + dy > 1) {
    return;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "drop_bomb: failed to load resources.");
    return;
  }

  uint8_t bomb_index = game->num_bombs;
  if (init_entity(&game->bombs[bomb_index], x, y, resources->bomb_sprites[0], 3) != 0) {
    fprintf(stderr, "drop_bomb: failed to initialize bomb entity.");
    return;
  }

  game->board.elements[y][x] = BOMB;
  game->bombs[bomb_index].active = true;
  game->num_bombs++;
}

void(explode_bomb)(Game *game, uint8_t bomb_index) {
  if (game == NULL || bomb_index >= game->num_bombs) {
    fprintf(stderr, "explode_bomb: invalid arguments.");
    return;
  }

  Entity *bomb = &game->bombs[bomb_index];

  if (!bomb->active) {
    fprintf(stderr, "explode_bomb: trying to explode inactive bomb.");
    return;
  }

  int16_t bomb_x = bomb->x;
  int16_t bomb_y = bomb->y;

  const int8_t dx[4] = {0, 1, 0, -1};
  const int8_t dy[4] = {-1, 0, 1, 0};

  for (int dir = 0; dir < 4; dir++) {
    for (int range = 1; range <= 2; range++) {
      int16_t cell_x = bomb_x + dx[dir] * range;
      int16_t cell_y = bomb_y + dy[dir] * range;

      if (cell_x < 0 || cell_x >= game->board.width ||
          cell_y < 0 || cell_y >= game->board.height) {
        break;
      }

      board_element_t cell = game->board.elements[cell_y][cell_x];

      switch (cell) {
        case PLAYER:
          if (game->player.x == cell_x && game->player.y == cell_y) {
            game->player.data--;
            if (game->player.data <= 0) {
              game->player.active = false;
              game->board.elements[cell_y][cell_x] = EMPTY_SPACE;
              game->state = LOSE;
            }
          }
          break;

        case ENEMY:
          for (uint8_t i = 0; i < game->num_enemies; i++) {
            if (game->enemies[i].active &&
                game->enemies[i].x == cell_x &&
                game->enemies[i].y == cell_y) {
              game->enemies[i].data--;
              if (game->enemies[i].data <= 0) {
                game->enemies[i].active = false;
                game->board.elements[cell_y][cell_x] = EMPTY_SPACE;
              }
              break;
            }
          }
          break;

        case BRICK:
          for (uint8_t i = 0; i < game->num_bricks; i++) {
            if (game->bricks[i].active &&
                game->bricks[i].x == cell_x &&
                game->bricks[i].y == cell_y) {
              game->bricks[i].data--;
              if (game->bricks[i].data <= 0) {
                game->bricks[i].active = false;
                game->board.elements[cell_y][cell_x] = EMPTY_SPACE;
                break;
              }
              game->bricks[i].sprite = get_resources()->brick_sprites[3 - game->bricks[i].data];
              break;
            }
          }
          break;

        case WALL:
          break;

        case BOMB:
          // TODO: explode other bombs
          break;

        case EMPTY_SPACE:
        case POWERUP:
          continue;

        default:
          fprintf(stderr, "explode_bomb: unknown board element type %d\n", cell);
          break;
      }

      if (cell == WALL || cell == BRICK || cell == BOMB) {
        break;
      }
    }
  }

  bomb->active = false;
  game->board.elements[bomb_y][bomb_x] = EMPTY_SPACE;

  uint8_t active_bricks = 0;
  for (uint8_t i = 0; i < game->num_bricks; i++) {
    if (game->bricks[i].active) {
      if (i != active_bricks) {
        game->bricks[active_bricks] = game->bricks[i];
      }
      active_bricks++;
    }
  }

  game->num_bricks = active_bricks;

  if (active_bricks == 0) {
    if (load_next_level(game) != 0) {
      game->state = WIN;
    }
  }
}

void(update_bombs)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "update_bombs: game pointer cannot be null.");
    return;
  }

  uint8_t active_bombs = 0;

  for (uint8_t i = 0; i < game->num_bombs; i++) {
    if (game->bombs[i].active) {
      game->bombs[i].data--;
      game->bombs[i].sprite = get_resources()->bomb_sprites[3 - game->bombs->data];

      if (game->bombs[i].data == 0) {
        explode_bomb(game, i);
      }
      else {
        if (i != active_bombs) {
          game->bombs[active_bombs] = game->bombs[i];
        }
        active_bombs++;
      }
    }
  }

  game->num_bombs = active_bombs;
}
