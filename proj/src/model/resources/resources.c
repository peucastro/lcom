#include <lcom/lcf.h>

#include "model/resources/resources.h"
#include "model/sprite/sprite.h"

#include "assets/xpm/wall.xpm"
#include "assets/xpm/brick.xpm"
#include "assets/xpm/player.xpm"
#include "assets/xpm/enemy.xpm"

Sprite *wall_sprite = NULL;
Sprite *brick_sprite = NULL;
Sprite *player_sprite = NULL;
Sprite *enemy_sprite = NULL;

int(create_resources)() {
  wall_sprite = create_sprite(wall_xpm, 0, 0);
  if (wall_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create wall sprite.");
    return 1;
  }

  brick_sprite = create_sprite(brick_xpm, 0, 0);
  if (brick_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create brick sprite.");
    return 1;
  }

  player_sprite = create_sprite(player_xpm, 0, 0);
  if (player_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create player sprite.");
    return 1;
  }

  enemy_sprite = create_sprite(enemy_xpm, 0, 0);
  if (enemy_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create enemy sprite.");
    return 1;
  }

  return 0;
}

void(destroy_resources)() {
  if (wall_sprite != NULL) {
    destroy_sprite(wall_sprite);
    wall_sprite = NULL;
  }

  if (brick_sprite != NULL) {
    destroy_sprite(brick_sprite);
    brick_sprite = NULL;
  }

  if (player_sprite != NULL) {
    destroy_sprite(player_sprite);
    player_sprite = NULL;
  }

  if (enemy_sprite != NULL) {
    destroy_sprite(enemy_sprite);
    enemy_sprite = NULL;
  }
}
