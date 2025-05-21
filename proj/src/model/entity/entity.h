#ifndef __PROJ_ENTITY_H
#define __PROJ_ENTITY_H

#include "model/sprite/sprite.h"

/** @defgroup entity entity
 * @{
 *
 * Module for managing game entities
 */

/**
 * @brief Struct representing an entity in the game
 *
 * An entity represents a game object with a position, a sprite, and a status.
 * Entities are stored in fixed-size arrays to avoid dynamic memory allocation.
 */
typedef struct {
  int16_t x, y;   /**< @brief x and y coordinates of the entity's position */
  Sprite *sprite; /**< @brief Pointer to the entity's sprite (not owned by the entity) */
  bool active;    /**< @brief Indicates whether the entity is currently active/valid in the game */
} Entity;

/**
 * @brief Initializes an entity with the given parameters
 *
 * Sets the entity's position and sprite, and marks it as active.
 *
 * @param entity Pointer to the entity to initialize
 * @param x The x-coordinate of the entity's position
 * @param y The y-coordinate of the entity's position
 * @param sp Pointer to the sprite associated with the entity
 *
 * @return 0 upon success, non-zero otherwise
 */
int(init_entity)(Entity *entity, int16_t x, int16_t y, Sprite *sp);

/**
 * @brief Resets an entity to its inactive state
 *
 * Clears the entity's position and marks it as inactive.
 *
 * @param entity Pointer to the entity to reset
 *
 * @return 0 upon success, non-zero otherwise
 */
int(reset_entity)(Entity *entity);

/**@}*/

#endif /* __PROJ_ENTITY_H */
