#ifndef __PROJ_RESOURCES_H
#define __PROJ_RESOURCES_H

#include "model/sprite/sprite.h"

#include "assets/xpm/bomb.xpm"
#include "assets/xpm/brick.xpm"
#include "assets/xpm/cursor.xpm"
#include "assets/xpm/enemy.xpm"
#include "assets/xpm/player_down.xpm"
#include "assets/xpm/player_left.xpm"
#include "assets/xpm/player_right.xpm"
#include "assets/xpm/player_up.xpm"
#include "assets/xpm/wall.xpm"

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
  Sprite *bomb_sprite;         /**< @brief Pointer to the bomb sprite */
  Sprite *brick_sprite;        /**< @brief Pointer to the brick sprite */
  Sprite *cursor_sprite;       /**< @brief Pointer to the brick sprite */
  Sprite *enemy_sprite;        /**< @brief Pointer to the enemy sprite */
  Sprite *player_down_sprite;  /**< @brief Pointer to the player down sprite */
  Sprite *player_left_sprite;  /**< @brief Pointer to the player left sprite */
  Sprite *player_right_sprite; /**< @brief Pointer to the player right sprite */
  Sprite *player_up_sprite;    /**< @brief Pointer to the player up sprite */
  Sprite *wall_sprite;         /**< @brief Pointer to the wall sprite */
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

/**@}*/

#endif /* __PROJ_RESOURCES_H */
