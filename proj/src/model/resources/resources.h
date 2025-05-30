#ifndef __PROJ_RESOURCES_H
#define __PROJ_RESOURCES_H

/* UI and interface elements */
#include "assets/xpm/ui/handopen.xpm"
#include "assets/xpm/ui/handpointing.xpm"
#include "assets/xpm/ui/heart.xpm"
#include "assets/xpm/ui/lose.xpm"
#include "assets/xpm/ui/menu_default.xpm"
#include "assets/xpm/ui/menu_exit.xpm"
#include "assets/xpm/ui/menu_pause.xpm"
#include "assets/xpm/ui/menu_pause_continue.xpm"
#include "assets/xpm/ui/menu_pause_exit.xpm"
#include "assets/xpm/ui/menu_start.xpm"
#include "assets/xpm/ui/win.xpm"

/* Game objects */
#include "assets/xpm/game/brick_0.xpm"
#include "assets/xpm/game/brick_1.xpm"
#include "assets/xpm/game/brick_2.xpm"
#include "assets/xpm/game/door.xpm"
#include "assets/xpm/game/powerup.xpm"
#include "assets/xpm/game/wall.xpm"

/* Text and fonts */
#include "assets/xpm/text/alphabet.xpm"
#include "assets/xpm/text/numbers.xpm"

#include "model/sprite/sprite.h"

/* Bomb animation cycle */
#include "assets/xpm/bomb/bomb_1.xpm"
#include "assets/xpm/bomb/bomb_2.xpm"
#include "assets/xpm/bomb/bomb_3.xpm"
#include "assets/xpm/bomb/bomb_4.xpm"

/* Enemy animation cycle */
#include "assets/xpm/enemy/enemy_a_1.xpm"
#include "assets/xpm/enemy/enemy_a_2.xpm"
#include "assets/xpm/enemy/enemy_a_3.xpm"
#include "assets/xpm/enemy/enemy_a_4.xpm"

/* Player animation cycles */
/* Down-facing run cycle */
#include "assets/xpm/player/player_down_1.xpm"
#include "assets/xpm/player/player_down_2.xpm"
#include "assets/xpm/player/player_down_3.xpm"
#include "assets/xpm/player/player_down_4.xpm"

/* Up-facing run cycle */
#include "assets/xpm/player/player_up_1.xpm"
#include "assets/xpm/player/player_up_2.xpm"
#include "assets/xpm/player/player_up_3.xpm"
#include "assets/xpm/player/player_up_4.xpm"

/* Left-facing run cycle */
#include "assets/xpm/player/player_left_1.xpm"
#include "assets/xpm/player/player_left_2.xpm"
#include "assets/xpm/player/player_left_3.xpm"
#include "assets/xpm/player/player_left_4.xpm"

/* Right-facing run cycle */
#include "assets/xpm/player/player_right_1.xpm"
#include "assets/xpm/player/player_right_2.xpm"
#include "assets/xpm/player/player_right_3.xpm"
#include "assets/xpm/player/player_right_4.xpm"

/* Explosion animation cycles */
/* Center explosion */
#include "assets/xpm/explosion/explosion_center_1.xpm"
#include "assets/xpm/explosion/explosion_center_2.xpm"
#include "assets/xpm/explosion/explosion_center_3.xpm"
#include "assets/xpm/explosion/explosion_center_4.xpm"

/* Horizontal explosion arms */
#include "assets/xpm/explosion/explosion_arm_h_1.xpm"
#include "assets/xpm/explosion/explosion_arm_h_2.xpm"
#include "assets/xpm/explosion/explosion_arm_h_3.xpm"
#include "assets/xpm/explosion/explosion_arm_h_4.xpm"

/* Vertical explosion arms */
#include "assets/xpm/explosion/explosion_arm_v_1.xpm"
#include "assets/xpm/explosion/explosion_arm_v_2.xpm"
#include "assets/xpm/explosion/explosion_arm_v_3.xpm"
#include "assets/xpm/explosion/explosion_arm_v_4.xpm"

/* Explosion end caps - directional */
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

#include "model/entity/entity.h"
#include "model/sprite/anim_sprite.h"

/** @brief Number of frames in enemy animation cycle */
#define ENEMY_ANIM_FRAMES 4
/** @brief Speed of enemy animation (ticks per frame) */
#define ENEMY_ANIM_SPEED 2

/** @brief Number of frames in player animation cycle */
#define PLAYER_ANIM_FRAMES 4
/** @brief Speed of player animation (ticks per frame) */
#define PLAYER_ANIM_SPEED 3

/** @brief Number of frames in bomb animation cycle */
#define BOMB_ANIM_FRAMES 4
/** @brief Speed of bomb animation (ticks per frame) */
#define BOMB_ANIM_SPEED 6

/** @brief Number of frames in explosion animation cycle */
#define EXPLOSION_ANIM_FRAMES 4
/** @brief Speed of explosion animation (ticks per frame) */
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
  Sprite *brick_sprites[3];                   /**< @brief Array of pointers to the brick sprites (damaged states) */
  Sprite *door_sprite;                        /**< @brief Pointer to the door sprite */
  Sprite *enemy_a_sprites[ENEMY_ANIM_FRAMES]; /**< @brief Array of pointers to the enemy animation sprites */
  Sprite *handopen_sprite;                    /**< @brief Pointer to the open hand cursor sprite */
  Sprite *handpointing_sprite;                /**< @brief Pointer to the pointing hand cursor sprite */
  Sprite *heart_sprite;                       /**< @brief Pointer to the heart/life indicator sprite */
  Sprite *lose_sprite;                        /**< @brief Pointer to the game over screen sprite */
  Sprite *menu_sprite[3];                     /**< @brief Array of pointers to the main menu sprites */
  Sprite *menu_pause[3];                      /**< @brief Array of pointers to the pause menu sprites */

  Sprite *player_up_sprites[PLAYER_ANIM_FRAMES];    /**< @brief Array of pointers to player upward movement sprites */
  Sprite *player_left_sprites[PLAYER_ANIM_FRAMES];  /**< @brief Array of pointers to player leftward movement sprites */
  Sprite *player_down_sprites[PLAYER_ANIM_FRAMES];  /**< @brief Array of pointers to player downward movement sprites */
  Sprite *player_right_sprites[PLAYER_ANIM_FRAMES]; /**< @brief Array of pointers to player rightward movement sprites */

  Sprite *bomb_sprites[BOMB_ANIM_FRAMES]; /**< @brief Array of pointers to bomb animation sprites */

  Sprite *explosion_center_sprites[EXPLOSION_ANIM_FRAMES];     /**< @brief Array of pointers to explosion center sprites */
  Sprite *explosion_horiz_sprites[EXPLOSION_ANIM_FRAMES];      /**< @brief Array of pointers to horizontal explosion arm sprites */
  Sprite *explosion_vert_sprites[EXPLOSION_ANIM_FRAMES];       /**< @brief Array of pointers to vertical explosion arm sprites */
  Sprite *explosion_hand_up_sprites[EXPLOSION_ANIM_FRAMES];    /**< @brief Array of pointers to upward explosion end cap sprites */
  Sprite *explosion_hand_down_sprites[EXPLOSION_ANIM_FRAMES];  /**< @brief Array of pointers to downward explosion end cap sprites */
  Sprite *explosion_hand_left_sprites[EXPLOSION_ANIM_FRAMES];  /**< @brief Array of pointers to leftward explosion end cap sprites */
  Sprite *explosion_hand_right_sprites[EXPLOSION_ANIM_FRAMES]; /**< @brief Array of pointers to rightward explosion end cap sprites */

  Sprite *powerup_sprite;  /**< @brief Pointer to the powerup sprite */
  Sprite *numbers_sprite;  /**< @brief Pointer to the numbers font sprite sheet */
  Sprite *alphabet_sprite; /**< @brief Pointer to the alphabet font sprite sheet */
  Sprite *win_sprite;      /**< @brief Pointer to the victory screen sprite */
  Sprite *wall_sprite;     /**< @brief Pointer to the wall sprite */
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

/**
 * @brief Retrieves the animated sprite for player movement in the specified direction
 *
 * Returns a pre-created AnimSprite object for the player's movement animation
 * in the given direction. These animations are initialized when resources are created.
 *
 * @param dir Direction for which to get the animation (UP, DOWN, LEFT, RIGHT)
 * @return Pointer to the AnimSprite for the specified direction
 */
AnimSprite *get_player_anim(Direction dir);

/**@}*/

#endif /* __PROJ_RESOURCES_H */
