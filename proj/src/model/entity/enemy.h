#ifndef __PROJ_ENEMY_H
#define __PROJ_ENEMY_H

#include "model/entity/entity.h"
#include "model/resources/resources.h"
#include "model/sprite/sprite.h"

/**
 * @brief Struct representing an enemy in the game
 *
 * Extends the base Entity struct with enemy-specific attributes.
 * Uses composition pattern with the Entity struct as its first member
 * to allow safe casting between Enemy* and Entity*.
 * Enemies move randomly around the board and can be eliminated by bombs.
 */
typedef struct Enemy {
  Entity base;    /**< @brief Base entity structure providing core functionality */
  int16_t health; /**< @brief Health points of the enemy, reduced when hit by explosions */
} Enemy;

/**
 * @brief Initializes an enemy with the given parameters
 *
 * Sets the enemy's position and sprite, marks it as active,
 * assigns the update_enemy function as the update handler,
 * and initializes enemy-specific attributes like health.
 *
 * @param e Pointer to the enemy to initialize
 * @param x The x-coordinate of the enemy's position
 * @param y The y-coordinate of the enemy's position
 * @param sp Pointer to the sprite associated with the enemy
 *
 * @return 0 upon success, non-zero otherwise
 */
int(init_enemy)(Enemy *e, int16_t x, int16_t y, Sprite *sp);

/**
 * @brief Resets an enemy to its inactive state
 *
 * Clears the enemy's entity data and resets enemy-specific
 * attributes like health to 0.
 *
 * @param e Pointer to the enemy to reset
 *
 * @return 0 upon success, non-zero otherwise
 */
int(reset_enemy)(Enemy *e);

/**
 * @brief Update function for enemy entities
 *
 * This function handles enemy-specific update logic and is assigned
 * to the on_update function pointer in the base Entity struct.
 * It generates random movement in one of four directions (up, right, down, left),
 * checks for collisions with board elements, and updates the enemy's position.
 * Enemies can move into empty spaces but are blocked by walls, bricks, bombs,
 * other enemies, and the player.
 *
 * @param e Pointer to the entity to update (cast to Enemy internally)
 * @param board Pointer to the game board for collision detection and movement validation
 * @param context Not used for enemies (can be NULL)
 */
void(update_enemy)(Entity *e, GameBoard *board, void *context);

#endif /* __PROJ_ENEMY_H */
