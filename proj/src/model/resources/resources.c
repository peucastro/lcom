#include <lcom/lcf.h>

#include "model/resources/resources.h"

static Resources resources;

static const char **(enemy_a_xpms)[ENEMY_ANIM_FRAMES] = {
  enemy_a_1_xpm,
  enemy_a_2_xpm,
  enemy_a_3_xpm,
  enemy_a_4_xpm};

int(create_resources)(void) {
  resources.bomb_sprite = create_sprite(bomb_xpm);
  if (resources.bomb_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create bomb sprite.");
    return 1;
  }

  resources.brick_sprites[0] = create_sprite(brick_0_xpm);
  resources.brick_sprites[1] = create_sprite(brick_1_xpm);
  resources.brick_sprites[2] = create_sprite(brick_2_xpm);
  for (uint8_t i = 0; i < 3; i++) {
    if (resources.brick_sprites[i] == NULL) {
      fprintf(stderr, "create_resources: failed to load brick sprite.");
      return 1;
    }
  }

  for (uint8_t i = 0; i < ENEMY_ANIM_FRAMES; i++) {
    resources.enemy_a_sprites[i] = create_sprite(enemy_a_xpms[i]);
    if (!resources.enemy_a_sprites[i]) {
      fprintf(stderr, "create_resources: failed to load enemy_%d\n", i);
      return 1;
    }
  }

  resources.handopen_sprite = create_sprite(handopen_xpm);
  if (resources.handopen_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create handopen sprite.");
    return 1;
  }

  resources.handpointing_sprite = create_sprite(handpointing_xpm);
  if (resources.handpointing_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create handpointing sprite.");
    return 1;
  }

  resources.menu_sprite[0] = create_sprite(menu_default_xpm);
  resources.menu_sprite[1] = create_sprite(menu_start_xpm);
  resources.menu_sprite[2] = create_sprite(menu_exit_xpm);
  for (uint8_t i = 0; i < 3; i++) {
    if (resources.menu_sprite[i] == NULL) {
      fprintf(stderr, "create_resources: failed to load menu sprite.");
      return 1;
    }
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

  for (uint8_t i = 0; i < 3; i++) {
    if (resources.brick_sprites[i]) {
      destroy_sprite(resources.brick_sprites[i]);
      resources.brick_sprites[i] = NULL;
    }
  }

  for (uint8_t i = 0; i < ENEMY_ANIM_FRAMES; i++) {
    if (resources.enemy_a_sprites[i]) {
      destroy_sprite(resources.enemy_a_sprites[i]);
      resources.enemy_a_sprites[i] = NULL;
    }
  }

  if (resources.handopen_sprite != NULL) {
    destroy_sprite(resources.handopen_sprite);
    resources.handopen_sprite = NULL;
  }

  if (resources.handpointing_sprite != NULL) {
    destroy_sprite(resources.handpointing_sprite);
    resources.handpointing_sprite = NULL;
  }

  for (uint8_t i = 0; i < 3; i++) {
    if (resources.menu_sprite[i]) {
      destroy_sprite(resources.menu_sprite[i]);
      resources.menu_sprite[i] = NULL;
    }
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
