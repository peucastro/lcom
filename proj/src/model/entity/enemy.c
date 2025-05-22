#include <lcom/lcf.h>

#include "model/entity/enemy.h"

int(init_enemy)(Enemy *e, int16_t x, int16_t y, Sprite *sp) {
  if (e == NULL) {
    fprintf(stderr, "init_enemy: enemy pointer cannot be null.");
    return 1;
  }

  if (sp == NULL) {
    fprintf(stderr, "init_enemy: sprite pointer cannot be null.");
    return 1;
  }

  if (init_entity(&e->base, x, y, sp) != 0) {
    fprintf(stderr, "init_enemy: failed to initialize base entity.");
    return 1;
  }

  e->base.on_update = update_enemy;
  e->health = 100;
  return 0;
}

int(reset_enemy)(Enemy *e) {
  if (e == NULL) {
    fprintf(stderr, "reset_enemy: enemy pointer cannot be null.");
    return 1;
  }

  if (reset_entity(&e->base) != 0) {
    fprintf(stderr, "reset_enemy: failed to reset entity.");
    return 1;
  }

  e->health = 0;
  return 0;
}

void(update_enemy)(Entity *e, GameBoard *board, void *context) {
  if (e == NULL || board == NULL) {
    fprintf(stderr, "update_enemy: invalid entity or board pointer.");
    return;
  }

  Enemy *enemy = (Enemy *) e;

  if (!enemy->base.active) {
    fprintf(stderr, "update_enemy: enemy not active.");
    return;
  }

  int direction = rand() % 4;

  int16_t new_x = enemy->base.x;
  int16_t new_y = enemy->base.y;

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

  if (new_x < 0 || new_x >= board->width ||
      new_y < 0 || new_y >= board->height) {
    return;
  }

  board_element_t destination = board->elements[new_y][new_x];

  switch (destination) {
    case EMPTY_SPACE:
    case POWERUP:
      board->elements[enemy->base.y][enemy->base.x] = EMPTY_SPACE;
      board->elements[new_y][new_x] = ENEMY;
      enemy->base.x = new_x;
      enemy->base.y = new_y;
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
