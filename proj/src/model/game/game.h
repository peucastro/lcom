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
  uint8_t menu_option;         /**< @brief Current selected menu option */
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
  uint8_t level;               /**< @brief Current game level */
} Game;

/**
 * @brief Loads the next level of the game
 *
 * Increments the level counter, loads the corresponding board file,
 * and initializes all entities according to the board layout.
 * Preserves the player's remaining lives between levels.
 *
 * @param game Pointer to the current game state
 *
 * @return 0 upon success, non-zero otherwise
 */
int(load_next_level)(Game *game);

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

/**
 * @brief Updates all active enemies in the game
 *
 * Iterates through all enemies in the game and moves each active one.
 * This function should be called periodically during gameplay.
 *
 * @param game Pointer to the current game state
 */
void(update_enemies)(Game *game);

/**
 * @brief Drops a bomb at the player's current position
 *
 * Places a bomb at the player's feet if possible. The bomb is placed on
 * the board and initialized as an entity. Bombs can only be placed on
 * empty spaces. The player remains standing on the newly placed bomb.
 *
 * @param game Pointer to the current game state
 *
 * @return 0 upon success, non-zero otherwise
 */
void(drop_bomb)(Game *game);

/**
 * @brief Causes a bomb to explode, damaging entities in its blast radius
 *
 * Creates an explosion that extends 3 tiles in each cardinal direction
 * (up, down, left, right). The explosion damages entities it encounters:
 * - Reduces player lives
 * - Reduces enemy lives
 * - Damages bricks until they break
 * - Stops at walls
 * - Can trigger chain reactions with other bombs
 *
 * @param game Pointer to the current game state
 * @param bomb_index Index of the bomb that is exploding
 */
void(explode_bomb)(Game *game, uint8_t bomb_index);

/**
 * @brief Updates the state of all active bombs
 *
 * This function is called periodically to update all bombs in the game.
 * For each active bomb, it decrements the countdown timer (stored in the data field).
 * When a bomb's timer reaches zero, the bomb is deactivated and removed from the board.
 * The function also compacts the bombs array by moving active bombs to fill gaps
 * left by deactivated bombs, and updates the total bomb count.
 *
 * @param game Pointer to the current game state
 */
void(update_bombs)(Game *game);

/**@}*/

#endif /* __PROJ_GAME_H */
