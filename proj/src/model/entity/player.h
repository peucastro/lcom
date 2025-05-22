#ifndef __PROJ_PLAYER_H
#define __PROJ_PLAYER_H

#include "model/entity/entity.h"
#include "model/resources/resources.h"
#include "model/sprite/sprite.h"

/**
 * @brief Struct representing a player in the game
 *
 * Extends the base Entity struct with player-specific attributes.
 * Uses composition pattern with the Entity struct as its first member
 * to allow safe casting between Player* and Entity*.
 */
typedef struct {
  Entity base;   /**< @brief Base entity structure providing core functionality */
  int16_t lives; /**< @brief Number of lives remaining for the player */
} Player;

/**
 * @brief Struct representing a player movement direction
 *
 * Contains horizontal and vertical movement values that indicate
 * the direction and magnitude of player movement requests.
 */
typedef struct {
  int16_t xmov; /**< @brief Horizontal movement (-1 for left, 0 for none, 1 for right) */
  int16_t ymov; /**< @brief Vertical movement (-1 for up, 0 for none, 1 for down) */
} PlayerMove;

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
 * It processes movement requests, handles collisions with board elements,
 * and updates the player's sprite based on movement direction.
 *
 * @param e Pointer to the entity to update (cast to Player internally)
 * @param board Pointer to the game board for collision detection and movement validation
 * @param context Pointer to a PlayerMove struct containing movement direction data
 */
void(update_player)(Entity *e, GameBoard *board, void *context);

#endif /* __PROJ_PLAYER_H */
