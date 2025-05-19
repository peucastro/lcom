#include <lcom/lcf.h>

#include "model/resources/resources.h"

static Resources resources;

int(create_resources)(void) {
  resources.bomb_sprite = create_sprite(bomb_xpm);
  if (resources.bomb_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create bomb sprite.");
    return 1;
  }

  resources.brick_sprite = create_sprite(brick_xpm);
  if (resources.brick_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create brick sprite.");
    return 1;
  }

  resources.enemy_sprite = create_sprite(enemy_xpm);
  if (resources.enemy_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create enemy sprite.");
    return 1;
  }

  resources.player_down_sprite = create_sprite(player_down_xpm);
  if (resources.player_down_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create player down sprite.");
    return 1;
  }

  resources.player_left_sprite = create_sprite(player_left_xpm);
  if (resources.player_left_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create player left sprite.");
    return 1;
  }

  resources.player_right_sprite = create_sprite(player_right_xpm);
  if (resources.player_right_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create player right sprite.");
    return 1;
  }

  resources.player_up_sprite = create_sprite(player_up_xpm);
  if (resources.player_up_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create player up sprite.");
    return 1;
  }

  resources.wall_sprite = create_sprite(wall_xpm);
  if (resources.wall_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create wall sprite.");
    return 1;
  }

  return 0;
}

void(destroy_resources)(void) {
  if (resources.bomb_sprite != NULL) {
    destroy_sprite(resources.bomb_sprite);
    resources.bomb_sprite = NULL;
  }

  if (resources.brick_sprite != NULL) {
    destroy_sprite(resources.brick_sprite);
    resources.brick_sprite = NULL;
  }

  if (resources.enemy_sprite != NULL) {
    destroy_sprite(resources.enemy_sprite);
    resources.enemy_sprite = NULL;
  }

  if (resources.player_down_sprite != NULL) {
    destroy_sprite(resources.player_down_sprite);
    resources.player_down_sprite = NULL;
  }

  if (resources.player_left_sprite != NULL) {
    destroy_sprite(resources.player_left_sprite);
    resources.player_left_sprite = NULL;
  }

  if (resources.player_right_sprite != NULL) {
    destroy_sprite(resources.player_right_sprite);
    resources.player_right_sprite = NULL;
  }

  if (resources.player_up_sprite != NULL) {
    destroy_sprite(resources.player_up_sprite);
    resources.player_up_sprite = NULL;
  }

  if (resources.wall_sprite != NULL) {
    destroy_sprite(resources.wall_sprite);
    resources.wall_sprite = NULL;
  }
}

const Resources *(get_resources) (void) {
  return &resources;
}
