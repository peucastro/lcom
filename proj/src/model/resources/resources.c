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

  resources.player_sprite = create_sprite(player_xpm);
  if (resources.player_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create player sprite.");
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

  if (resources.player_sprite != NULL) {
    destroy_sprite(resources.player_sprite);
    resources.player_sprite = NULL;
  }

  if (resources.wall_sprite != NULL) {
    destroy_sprite(resources.wall_sprite);
    resources.wall_sprite = NULL;
  }
}

const Resources *(get_resources) (void) {
  return &resources;
}
