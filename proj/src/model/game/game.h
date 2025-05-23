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

#define MAX_ENEMIES 10 /**< @brief Maximum number of enemies allowed in the game */
#define MAX_BRICKS 50  /**< @brief Maximum number of bricks allowed in the game */
#define MAX_WALLS 150  /**< @brief Maximum number of walls allowed in the game */
#define MAX_BOMBS 5    /**< @brief Maximum number of bombs that can exist simultaneously */

/**
 * @brief Enum representing the game state
 */
typedef enum {
  START, /**< @brief Game is in start menu */
  PAUSE, /**< @brief Game is in pause menu */
  GAME,  /**< @brief Game is being played */
  EXIT   /**< @brief Game is exiting */
} game_state_t;

/**
 * @brief Struct representing the game
 *
 * This struct contains the current state of the game, the game board,
 * and all game entities (player, enemies, bricks, walls, bombs).
 * All entities are stored in fixed-size arrays to avoid dynamic memory allocation.
 */
typedef struct {
  game_state_t state;          /**< @brief Current state of the game */
  GameBoard board;             /**< @brief Current game board */
  Entity player;               /**< @brief The player entity */
  Entity enemies[MAX_ENEMIES]; /**< @brief Array of enemy entities */
  Entity bricks[MAX_BRICKS];   /**< @brief Array of brick entities */
  Entity walls[MAX_WALLS];     /**< @brief Array of wall entities */
  Entity bombs[MAX_BOMBS];     /**< @brief Array of bomb entities */
  uint8_t num_enemies;         /**< @brief Number of enemies in the game */
  uint8_t num_bricks;          /**< @brief Number of bricks in the game */
  uint8_t num_walls;           /**< @brief Number of walls in the game */
  uint8_t num_bombs;           /**< @brief Number of bombs in the game */
} Game;

/**
 * @brief Initializes the game
 *
 * Sets up the game state and initializes the game board,
 * player, enemies, bricks, walls, and bombs based on the board layout.
 * Uses stack-allocated entities to avoid dynamic memory allocation.
 *
 * @param game Pointer to the game to be initialized
 *
 * @return 0 upon success, non-zero otherwise
 */
int(init_game)(Game *game);

/**
 * @brief Resets the game
 *
 * Resets all entities in the game to their default state and
 * clears the game board.
 *
 * @param game Pointer to the game to be reset
 *
 * @return 0 upon success, non-zero otherwise
 */
int(reset_game)(Game *game);

/**
 * @brief Updates the player entity based on movement input
 *
 *
 * @param p Pointer to the player entity
 * @param game Pointer to the current game state
 * @param xmov Horizontal movement (-1: left, 0: none, 1: right)
 * @param ymov Vertical movement (-1: up, 0: none, 1: down)
 */
void(move_player)(Entity *p, Game *game, int16_t xmov, int16_t ymov);

/**
 * @brief Updates an enemy entity with random movement
 *
 * Generates random movement in one of four directions (up, right, down, left).
 * Only moves to empty spaces, powerups or the player.
 *
 * @param e Pointer to the enemy entity
 * @param game Pointer to the current game state
 */
void(move_enemy)(Entity *e, Game *game);

void(drop_bomb)(Game *game, int16_t x, int16_t y);

/**@}*/

#endif /* __PROJ_GAME_H */
