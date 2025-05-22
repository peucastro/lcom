#ifndef __PROJ_PLAYER_H
#define __PROJ_PLAYER_H

#include "model/entity/entity.h"
#include "model/sprite/sprite.h"

/**
 * @brief Struct representing a player in the game
 *
 * Extends the base Entity struct with player-specific attributes.
 */
typedef struct {
  Entity base;   /**< @brief Base entity structure */
  int16_t lives; /**< @brief Number of lives remaining for the player */
} Player;

/**
 * @brief Initializes a player with the given parameters
 *
 * Sets the player's position and sprite, marks it as active,
 * assigns the update_player function as the update handler,
 * and initializes player-specific attributes like lives.
 *
 * @param p Pointer to the player to initialize
 * @param x The x-coordinate of the player's position
 * @param y The y-coordinate of the player's position
 * @param sp Pointer to the sprite associated with the player
 *
 * @return 0 upon success, non-zero otherwise
 */
int(init_player)(Player *p, int16_t x, int16_t y, Sprite *sp);

/**
 * @brief Resets a player to its inactive state
 *
 * Clears the player's entity data and resets player-specific
 * attributes like lives count to 0.
 *
 * @param p Pointer to the player to reset
 *
 * @return 0 upon success, non-zero otherwise
 */
int(reset_player)(Player *p);

/**
 * @brief Update function for player entities
 *
 * This function handles player-specific update logic and is assigned
 * to the on_update function pointer in the base Entity struct.
 *
 * @param p Pointer to the entity to update (cast to Player as needed)
 */
void(update_player)(struct Entity *p);

#endif /* __PROJ_PLAYER_H */
