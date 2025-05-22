#include <lcom/lcf.h>

#include "model/entity/player.h"

int(init_player)(Player *p, int16_t x, int16_t y, Sprite *sp) {
  if (p == NULL) {
    fprintf(stderr, "init_player: player pointer cannot be null.");
    return 1;
  }

  if (sp == NULL) {
    fprintf(stderr, "init_player: sprite pointer cannot be null.");
    return 1;
  }

  if (init_entity(&p->base, x, y, sp) != 0) {
    fprintf(stderr, "init_player: failed to initialize base entity.");
    return 1;
  }

  p->base.on_update = update_player;
  p->lives = 3;
  return 0;
}

int(reset_player)(Player *p) {
  if (p == NULL) {
    fprintf(stderr, "reset_player: player pointer cannot be null.");
    return 1;
  }

  if (reset_entity(&p->base) != 0) {
    fprintf(stderr, "reset_player: failed to reset entity.");
    return 1;
  }

  p->lives = 0;
  return 0;
}

void(update_player)(Entity *e, GameBoard *board, void *context) {
  if (e == NULL || board == NULL) {
    fprintf(stderr, "update_player: invalid entity or board pointer.");
    return;
  }

  Player *p = (Player *) e;
  PlayerMove *move = (PlayerMove *) context;

  if (!p->base.active) {
    fprintf(stderr, "update_player: player not active.");
    return;
  }

  int16_t new_x = p->base.x + move->xmov;
  int16_t new_y = p->base.y + move->ymov;

  if (new_x < 0 || new_x >= board->width ||
      new_y < 0 || new_y >= board->height) {
    fprintf(stderr, "update_player: invalid coordinate.");
    return;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "update_player: failed to load resources.");
    return;
  }

  if (move->xmov > 0) {
    p->base.sprite = resources->player_right_sprite;
  }
  else if (move->xmov < 0) {
    p->base.sprite = resources->player_left_sprite;
  }
  else if (move->ymov > 0) {
    p->base.sprite = resources->player_down_sprite;
  }
  else if (move->ymov < 0) {
    p->base.sprite = resources->player_up_sprite;
  }

  board_element_t destination = board->elements[new_y][new_x];

  switch (destination) {
    case EMPTY_SPACE:
    case POWERUP:
      board->elements[p->base.y][p->base.x] = EMPTY_SPACE;
      board->elements[new_y][new_x] = PLAYER;
      p->base.x = new_x;
      p->base.y = new_y;
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
