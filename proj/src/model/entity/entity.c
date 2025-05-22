#include <lcom/lcf.h>

#include "model/entity/entity.h"
#include "model/game/game.h"

int(init_entity)(Entity *e, int16_t x, int16_t y, Sprite *sp, int16_t data, UpdateFunc update_func) {
  if (e == NULL) {
    fprintf(stderr, "init_entity: entity pointer cannot be null.");
    return 1;
  }

  if (sp == NULL) {
    fprintf(stderr, "init_entity: sprite pointer cannot be null.");
    return 1;
  }

  e->x = x;
  e->y = y;
  e->sprite = sp;
  e->active = true;
  e->data = data;
  e->on_update = update_func;

  return 0;
}

int(reset_entity)(Entity *e) {
  if (e == NULL) {
    fprintf(stderr, "reset_entity: entity pointer cannot be null.");
    return 1;
  }

  e->x = 0;
  e->y = 0;
  e->sprite = NULL;
  e->active = false;
  e->on_update = NULL;

  return 0;
}

void(update_player)(Entity *e, void *g, void *context) {
  if (e == NULL || g == NULL) {
    fprintf(stderr, "update_player: invalid entity or game pointer.");
    return;
  }

  PlayerMove *move = (PlayerMove *) context;
  Game *game = (Game *) g;

  if (!e->active) {
    fprintf(stderr, "update_player: player not active.");
    return;
  }

  int16_t new_x = e->x + move->xmov;
  int16_t new_y = e->y + move->ymov;

  if (new_x < 0 || new_x >= game->board.width ||
      new_y < 0 || new_y >= game->board.height) {
    fprintf(stderr, "update_player: invalid coordinate.");
    return;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "update_player: failed to load resources.");
    return;
  }

  if (move->xmov > 0) {
    e->sprite = resources->player_right_sprite;
  }
  else if (move->xmov < 0) {
    e->sprite = resources->player_left_sprite;
  }
  else if (move->ymov > 0) {
    e->sprite = resources->player_down_sprite;
  }
  else if (move->ymov < 0) {
    e->sprite = resources->player_up_sprite;
  }

  board_element_t destination = game->board.elements[new_y][new_x];

  switch (destination) {
    case EMPTY_SPACE:
    case POWERUP:
      game->board.elements[e->y][e->x] = EMPTY_SPACE;
      game->board.elements[new_y][new_x] = PLAYER;
      e->x = new_x;
      e->y = new_y;
      break;

    case WALL:
    case BRICK:
    case BOMB:
    case ENEMY:
      // block movement
      break;
    default:
      fprintf(stderr, "update_player: invalid destination.");
      return;
  }
}

void(update_enemy)(Entity *e, void *g, void *context) {
  if (e == NULL || g == NULL) {
    fprintf(stderr, "update_enemy: invalid entity or game pointer.");
    return;
  }

  if (!e->active) {
    fprintf(stderr, "update_enemy: enemy not active.");
    return;
  }

  Game *game = (Game *) g;

  int direction = rand() % 4;

  int16_t new_x = e->x;
  int16_t new_y = e->y;

  switch (direction) {
    case 0: // up
      new_y--;
      break;
    case 1: // right
      new_x++;
      break;
    case 2: // down
      new_y++;
      break;
    case 3: // left
      new_x--;
      break;
  }

  if (new_x < 0 || new_x >= game->board.width ||
      new_y < 0 || new_y >= game->board.height) {
    return;
  }

  board_element_t destination = game->board.elements[new_y][new_x];

  switch (destination) {
    case EMPTY_SPACE:
    case POWERUP:
      game->board.elements[e->y][e->x] = EMPTY_SPACE;
      game->board.elements[new_y][new_x] = ENEMY;
      e->x = new_x;
      e->y = new_y;
      break;

    case WALL:
    case BRICK:
    case BOMB:
    case ENEMY:
    case PLAYER:
      // don't move if blocked
      break;
    default:
      fprintf(stderr, "update_enemy: invalid destination.");
      return;
  }
}
