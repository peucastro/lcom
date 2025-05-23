#ifndef __PROJ_ENTITY_H
#define __PROJ_ENTITY_H

#include "model/board/board.h"
#include "model/sprite/sprite.h"

/** @defgroup entity entity
 * @{
 *
 * Module for managing game entities
 *
 * Provides core functionality for game objects like players, enemies, and bombs.
 * Defines the base Entity struct and common operations for all entity types.
 */

/**
 * @brief Enum representing entity facing directions
 */
typedef enum {
  UP,    /**< @brief Entity is facing upward */
  RIGHT, /**< @brief Entity is facing right */
  DOWN,  /**< @brief Entity is facing downward */
  LEFT   /**< @brief Entity is facing left */
} Direction;

/**
 * @brief Base game entity structure
 *
 * Represents any game object with position, visual representation, and behavior.
 */
typedef struct Entity {
  int16_t x, y;   /**< Position coordinates */
  Direction dir;  /**< Entity's facing direction */
  Sprite *sprite; /**< Visual representation */
  bool active;    /**< Whether entity is currently in play */
  int16_t data;   /**< Entity-specific data (lives, damage, etc.) */
} Entity;

/**
 * @brief Initialize an entity
 *
 * @param e Entity to initialize
 * @param x X-coordinate
 * @param y Y-coordinate
 * @param sp Sprite for visual representation
 * @param data Entity-specific data value
 *
 * @return 0 on success, non-zero otherwise
 */
int(init_entity)(Entity *e, int16_t x, int16_t y, Sprite *sp, int16_t data);

/**
 * @brief Reset an entity to inactive state
 *
 * @param e Entity to reset
 * @return 0 on success, non-zero otherwise
 */
int(reset_entity)(Entity *e);

/**@}*/

#endif /* __PROJ_ENTITY_H */
