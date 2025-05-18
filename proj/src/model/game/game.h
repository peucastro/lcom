#ifndef __PROJ_GAME_H
#define __PROJ_GAME_H

#include "model/board/board.h"
#include "model/entity/entity.h"
#include "model/resources/resources.h"
#include "model/sprite/sprite.h"

/** @defgroup game game
 * @{
 *
 * Functions for managing the game state
 */

/**
 * @brief Enum representing the game state
 */
typedef enum {
  START, /**< @brief Game is in start menu */
  PAUSE, /**< @brief Game is in pause menu */
  GAME,  /**< @brief Game is being played */
  EXIT   /**< @brief Game is exiting */
} game_state;

/**
 * @brief Struct representing the game
 *
 * This struct contains the current state of the game, the game board,
 * and pointers to the player, enemies, bricks, and walls.
 */
typedef struct {
  game_state state;    /**< @brief Current state of the game */
  GameBoard *board;    /**< @brief Current game board */
  Entity *player;      /**< @brief Pointer to the player entity */
  Entity **enemies;    /**< @brief Array of pointers to enemy entities */
  Entity **bricks;     /**< @brief Array of pointers to brick entities */
  Entity **walls;      /**< @brief Array of pointers to wall entities */
  Entity **bombs;      /**< @brief Array of pointers to bomb entities */
  uint8_t num_enemies; /**< @brief Number of enemies in the game */
  uint8_t num_bricks;  /**< @brief Number of bricks in the game */
  uint8_t num_walls;   /**< @brief Number of walls in the game */
  uint8_t num_bombs;   /**< @brief Number of bombs in the game */
} Game;

/**
 * @brief Initializes the game
 *
 * Sets up the game state, allocates resources, and initializes
 * the game board, player, enemies, bricks, and walls.
 *
 * @param game Pointer to the game to be initialized
 *
 * @return 0 upon success, non-zero otherwise
 */
int(init_game)(Game *game);

/**
 * @brief Destroys the game
 *
 * Frees all resources allocated for the game, including the game board,
 * player, enemies, bricks, and walls.
 *
 * @param game Pointer to the game to be destroyed
 *
 * @return 0 upon success, non-zero otherwise
 */
int(destroy_game)(Game *game);

/**@}*/

#endif /* __PROJ_GAME_H */
