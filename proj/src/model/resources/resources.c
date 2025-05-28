#include <lcom/lcf.h>

#include "model/resources/resources.h"

static Resources resources;

static const char **(enemy_a_xpms)[ENEMY_ANIM_FRAMES] = {
  enemy_a_1_xpm,
  enemy_a_2_xpm,
  enemy_a_3_xpm,
  enemy_a_4_xpm
};

static const char **player_down_xpms[PLAYER_ANIM_FRAMES] = { player_down_1_xpm, player_down_2_xpm, player_down_3_xpm, player_down_4_xpm };
static const char **player_up_xpms[PLAYER_ANIM_FRAMES] = { player_up_1_xpm, player_up_2_xpm, player_up_3_xpm, player_up_4_xpm };
static const char **player_left_xpms[PLAYER_ANIM_FRAMES] = { player_left_1_xpm, player_left_2_xpm, player_left_3_xpm, player_left_4_xpm };
static const char **player_right_xpms[PLAYER_ANIM_FRAMES] = { player_right_1_xpm, player_right_2_xpm, player_right_3_xpm, player_right_4_xpm };

static const char **bomb_xpms[BOMB_ANIM_FRAMES] = { bomb_1_xpm, bomb_2_xpm, bomb_3_xpm, bomb_4_xpm };

int(create_resources)(void) {
   for (uint8_t i = 0; i < BOMB_ANIM_FRAMES; i++) {
    resources.bomb_sprites[i] = create_sprite(bomb_xpms[i]);
    if (!resources.bomb_sprites[i]) {
      fprintf(stderr, "create_resources: failed to load bomb anim frame %u\n", i);
      return 1;
    }
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

  resources.lose_sprite = create_sprite(lose_xpm);
  if (resources.lose_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create lose sprite.");
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

  for (uint8_t i = 0; i < PLAYER_ANIM_FRAMES; i++) {
    resources.player_down_sprites [i] = create_sprite(player_down_xpms [i]);
    resources.player_up_sprites [i] = create_sprite(player_up_xpms [i]);
    resources.player_left_sprites [i] = create_sprite(player_left_xpms [i]);
    resources.player_right_sprites[i] = create_sprite(player_right_xpms[i]);
    if (!resources.player_down_sprites[i] || !resources.player_up_sprites[i] || !resources.player_left_sprites[i] || !resources.player_right_sprites[i]) {
      fprintf(stderr, "create_resources: failed to load player anim frame %u\n", i);
      return 1;
    }
  }

  resources.powerup_sprite = create_sprite(powerup_xpm);
  if (resources.powerup_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create powerup sprite.");
    return 1;
  }

  resources.wall_sprite = create_sprite(wall_xpm);
  if (resources.wall_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create wall sprite.");
    return 1;
  }

  resources.win_sprite = create_sprite(win_xpm);
  if (resources.win_sprite == NULL) {
    fprintf(stderr, "create_resources: failed to create win sprite.");
    return 1;
  }

  return 0;
}

void(destroy_resources)(void) {
  for (uint8_t i = 0; i < BOMB_ANIM_FRAMES; i++) {
    if (resources.bomb_sprites[i]) {
      destroy_sprite(resources.bomb_sprites[i]);
      resources.bomb_sprites[i] = NULL;
    }
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

  if (resources.lose_sprite != NULL) {
    destroy_sprite(resources.lose_sprite);
    resources.lose_sprite = NULL;
  }

  for (uint8_t i = 0; i < 3; i++) {
    if (resources.menu_sprite[i]) {
      destroy_sprite(resources.menu_sprite[i]);
      resources.menu_sprite[i] = NULL;
    }
  }

  for (uint8_t i = 0; i < PLAYER_ANIM_FRAMES; i++) {
    if (resources.player_down_sprites [i] != NULL) {
      destroy_sprite(resources.player_down_sprites [i]);
      resources.player_down_sprites [i] = NULL;
    }
    if (resources.player_up_sprites [i] != NULL) {
      destroy_sprite(resources.player_up_sprites   [i]);
      resources.player_up_sprites   [i] = NULL;
    }
    if (resources.player_left_sprites [i] != NULL) {
      destroy_sprite(resources.player_left_sprites [i]);
      resources.player_left_sprites [i] = NULL;
    }
    if (resources.player_right_sprites[i] != NULL) {
      destroy_sprite(resources.player_right_sprites[i]);
      resources.player_right_sprites[i] = NULL;
    }
  }

  if (resources.powerup_sprite != NULL) {
    destroy_sprite(resources.powerup_sprite);
    resources.powerup_sprite = NULL;
  }

  if (resources.wall_sprite != NULL) {
    destroy_sprite(resources.wall_sprite);
    resources.wall_sprite = NULL;
  }

  if (resources.lose_sprite != NULL) {
    destroy_sprite(resources.lose_sprite);
    resources.lose_sprite = NULL;
  }
}

const Resources *(get_resources) (void) {
  return &resources;
}
