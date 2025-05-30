#ifndef __PROJ_RESOURCES_H
#define __PROJ_RESOURCES_H

#include "assets/xpm/brick_0.xpm"
#include "assets/xpm/brick_1.xpm"
#include "assets/xpm/brick_2.xpm"
#include "assets/xpm/door.xpm"
#include "assets/xpm/handopen.xpm"
#include "assets/xpm/handpointing.xpm"
#include "assets/xpm/heart.xpm"
#include "assets/xpm/lose.xpm"
#include "assets/xpm/menu_default.xpm"
#include "assets/xpm/menu_exit.xpm"
#include "assets/xpm/menu_start.xpm"
#include "assets/xpm/menu_pause.xpm"
#include "assets/xpm/menu_pause_continue.xpm"
#include "assets/xpm/menu_pause_exit.xpm"
#include "assets/xpm/powerup.xpm"
#include "assets/xpm/score.xpm"
#include "assets/xpm/wall.xpm"
#include "assets/xpm/win.xpm"
#include "model/sprite/sprite.h"

/* bomb cycle */
#include "assets/xpm/bomb/bomb_1.xpm"
#include "assets/xpm/bomb/bomb_2.xpm"
#include "assets/xpm/bomb/bomb_3.xpm"
#include "assets/xpm/bomb/bomb_4.xpm"

/* enemy cycle */
#include "assets/xpm/enemy/enemy_a_1.xpm"
#include "assets/xpm/enemy/enemy_a_2.xpm"
#include "assets/xpm/enemy/enemy_a_3.xpm"
#include "assets/xpm/enemy/enemy_a_4.xpm"

/* player cycle */
/* down-facing run cycle */
#include "assets/xpm/player/player_down_1.xpm"
#include "assets/xpm/player/player_down_2.xpm"
#include "assets/xpm/player/player_down_3.xpm"
#include "assets/xpm/player/player_down_4.xpm"

/* up-facing run cycle */
#include "assets/xpm/player/player_up_1.xpm"
#include "assets/xpm/player/player_up_2.xpm"
#include "assets/xpm/player/player_up_3.xpm"
#include "assets/xpm/player/player_up_4.xpm"

/* left-facing run cycle */
#include "assets/xpm/player/player_left_1.xpm"
#include "assets/xpm/player/player_left_2.xpm"
#include "assets/xpm/player/player_left_3.xpm"
#include "assets/xpm/player/player_left_4.xpm"

/* right-facing run cycle */
#include "assets/xpm/player/player_right_1.xpm"
#include "assets/xpm/player/player_right_2.xpm"
#include "assets/xpm/player/player_right_3.xpm"
#include "assets/xpm/player/player_right_4.xpm"

/* explosion cycle */
#include "assets/xpm/explosion/explosion_center_1.xpm"
#include "assets/xpm/explosion/explosion_center_2.xpm"
#include "assets/xpm/explosion/explosion_center_3.xpm"
#include "assets/xpm/explosion/explosion_center_4.xpm"

#include "assets/xpm/explosion/explosion_arm_h_1.xpm"
#include "assets/xpm/explosion/explosion_arm_h_2.xpm"
#include "assets/xpm/explosion/explosion_arm_h_3.xpm"
#include "assets/xpm/explosion/explosion_arm_h_4.xpm"

#include "assets/xpm/explosion/explosion_arm_v_1.xpm"
#include "assets/xpm/explosion/explosion_arm_v_2.xpm"
#include "assets/xpm/explosion/explosion_arm_v_3.xpm"
#include "assets/xpm/explosion/explosion_arm_v_4.xpm"

#include "assets/xpm/explosion/explosion_hand_right_1.xpm"
#include "assets/xpm/explosion/explosion_hand_right_2.xpm"
#include "assets/xpm/explosion/explosion_hand_right_3.xpm"
#include "assets/xpm/explosion/explosion_hand_right_4.xpm"

#include "assets/xpm/explosion/explosion_hand_left_1.xpm"
#include "assets/xpm/explosion/explosion_hand_left_2.xpm"
#include "assets/xpm/explosion/explosion_hand_left_3.xpm"
#include "assets/xpm/explosion/explosion_hand_left_4.xpm"

#include "assets/xpm/explosion/explosion_hand_up_1.xpm"
#include "assets/xpm/explosion/explosion_hand_up_2.xpm"
#include "assets/xpm/explosion/explosion_hand_up_3.xpm"
#include "assets/xpm/explosion/explosion_hand_up_4.xpm"

#include "assets/xpm/explosion/explosion_hand_down_1.xpm"
#include "assets/xpm/explosion/explosion_hand_down_2.xpm"
#include "assets/xpm/explosion/explosion_hand_down_3.xpm"
#include "assets/xpm/explosion/explosion_hand_down_4.xpm"

#include "model/sprite/anim_sprite.h"
#include "model/entity/entity.h"

#define ENEMY_ANIM_FRAMES 4
#define ENEMY_ANIM_SPEED 2

#define PLAYER_ANIM_FRAMES 4
#define PLAYER_ANIM_SPEED 3

#define BOMB_ANIM_FRAMES 4
#define BOMB_ANIM_SPEED 6

#define EXPLOSION_ANIM_FRAMES 4
#define EXPLOSION_ANIM_SPEED 4

/** @defgroup resources resources
 * @{
 *
 * Functions for managing game resources
 */

/**
 * @brief Struct representing game resources
 *
 * This struct contains pointers to all the sprites used in the game,
 * including the wall, brick, player, and enemy sprites.
 */
typedef struct {
  Sprite *brick_sprites[3];                   /**< @brief Array of pointer to the brick sprites */
  Sprite *door_sprite;                        /**< @brief Pointer to the door sprite */
  Sprite *enemy_a_sprites[ENEMY_ANIM_FRAMES]; /**< @brief Array of pointer to the enemy sprites */
  Sprite *handopen_sprite;                    /**< @brief Pointer to the handopen sprite */
  Sprite *handpointing_sprite;                /**< @brief Pointer to the handpointing sprite */
  Sprite *heart_sprite;                       /**< @brief Pointer to the heart sprite */
  Sprite *lose_sprite;                        /**< @brief Pointer to the lose sprite */
  Sprite *menu_sprite[3];                     /**< @brief Array of pointers to the menu sprites */
  Sprite *menu_pause[3];

  Sprite *player_up_sprites[PLAYER_ANIM_FRAMES];
  Sprite *player_left_sprites[PLAYER_ANIM_FRAMES];
  Sprite *player_down_sprites[PLAYER_ANIM_FRAMES];
  Sprite *player_right_sprites[PLAYER_ANIM_FRAMES];

  Sprite *bomb_sprites[BOMB_ANIM_FRAMES];

  Sprite *explosion_center_sprites[EXPLOSION_ANIM_FRAMES];
  Sprite *explosion_horiz_sprites[EXPLOSION_ANIM_FRAMES];
  Sprite *explosion_vert_sprites[EXPLOSION_ANIM_FRAMES];
  Sprite *explosion_hand_up_sprites[EXPLOSION_ANIM_FRAMES];
  Sprite *explosion_hand_down_sprites[EXPLOSION_ANIM_FRAMES];
  Sprite *explosion_hand_left_sprites[EXPLOSION_ANIM_FRAMES];
  Sprite *explosion_hand_right_sprites[EXPLOSION_ANIM_FRAMES];

  Sprite *powerup_sprite; /**< @brief Pointer to the powerup sprite */
  Sprite *score_sprite;   /**< @brief Pointer to the score text sprite */
  Sprite *win_sprite;     /**< @brief Pointer to the win sprite */
  Sprite *wall_sprite;    /**< @brief Pointer to the wall sprite */
} Resources;

/**
 * @brief Creates all game resources
 *
 * Allocates and initializes all the sprites used in the game, including
 * the wall, brick, player, and enemy sprites.
 *
 * @return 0 upon success, non-zero otherwise
 */
int(create_resources)(void);

/**
 * @brief Destroys all game resources
 *
 * Frees the memory allocated for all the sprites used in the game.
 */
void(destroy_resources)(void);

/**
 * @brief Retrieves the game resources
 *
 * Provides access to the `Resources` struct containing all the game sprites.
 *
 * @return Pointer to the `Resources` struct
 */
const Resources *(get_resources) (void);


AnimSprite* get_player_anim(Direction dir);

/**@}*/

#endif /* __PROJ_RESOURCES_H */
