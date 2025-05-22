#ifndef __PROJ_ENTITY_H
#define __PROJ_ENTITY_H

#include "model/board/board.h"
#include "model/sprite/sprite.h"

/** @defgroup entity entity
 * @{
 *
 * Module for managing game entities
 *
 * This module provides the core entity functionality for the game,
 * defining the base Entity struct that other game objects extend.
 * It handles common entity operations like initialization and reset.
 */

struct Entity; // forward-declaration

/**
 * @brief Function pointer type for entity-specific update behavior
 *
 * Defines the signature for entity update functions that handle
 * entity-specific logic during game updates. Different entity types
 * will implement their own update functions following this signature.
 *
 * @param self Pointer to the entity being updated
 * @param board Pointer to the game board for spatial queries
 * @param context Optional additional data needed for updates (movement info, etc.)
 */
typedef void (*UpdateFunc)(struct Entity *self, GameBoard *board, void *context);

/**
 * @brief Struct representing an entity in the game
 *
 * An entity represents a game object with a position, a sprite, and a status.
 * Entities are stored in fixed-size arrays to avoid dynamic memory allocation.
 * This serves as the base structure for all game objects (player, enemies, etc.).
 */
typedef struct Entity {
  int16_t x, y;         /**< @brief x and y coordinates of the entity's position */
  Sprite *sprite;       /**< @brief Pointer to the entity's sprite (not owned by the entity) */
  bool active;          /**< @brief Indicates whether the entity is currently active/valid in the game */
  UpdateFunc on_update; /**< @brief Entity-specific update function called during game loop */
} Entity;

/**
 * @brief Initializes an entity with the given parameters
 *
 * Sets the entity's position and sprite, and marks it as active.
 * The update function is not assigned here and should be set by derived entities.
 *
 * @param e Pointer to the entity to initialize
 * @param x The x-coordinate of the entity's position
 * @param y The y-coordinate of the entity's position
 * @param sp Pointer to the sprite associated with the entity
 *
 * @return 0 upon success, non-zero otherwise
 */
int(init_entity)(Entity *e, int16_t x, int16_t y, Sprite *sp);

/**
 * @brief Resets an entity to its inactive state
 *
 * Clears the entity's position and marks it as inactive.
 * This prepares the entity for reuse or removal from the game.
 *
 * @param e Pointer to the entity to reset
 *
 * @return 0 upon success, non-zero otherwise
 */
int(reset_entity)(Entity *e);

/**@}*/

#endif /* __PROJ_ENTITY_H */
