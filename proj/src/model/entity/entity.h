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

struct Entity; // forward-declaration

/**
 * @brief Function pointer type for entity-specific behavior
 *
 * @param self Entity being updated
 * @param g Game state containing the board and other entities
 * @param context Optional data needed for updates (e.g., movement direction)
 */
typedef void (*UpdateFunc)(struct Entity *self, void *g, void *context);

/**
 * @brief Movement direction for player entities
 */
typedef struct {
  int16_t xmov; /**< Horizontal movement (-1: left, 0: none, 1: right) */
  int16_t ymov; /**< Vertical movement (-1: up, 0: none, 1: down) */
} PlayerMove;

/**
 * @brief Base game entity structure
 *
 * Represents any game object with position, visual representation, and behavior.
 */
typedef struct Entity {
  int16_t x, y;         /**< Position coordinates */
  Sprite *sprite;       /**< Visual representation */
  bool active;          /**< Whether entity is currently in play */
  int16_t data;         /**< Entity-specific data (lives, damage, etc.) */
  UpdateFunc on_update; /**< Entity behavior function */
} Entity;

/**
 * @brief Initialize an entity
 *
 * @param e Entity to initialize
 * @param x X-coordinate
 * @param y Y-coordinate
 * @param sp Sprite for visual representation
 * @param data Entity-specific data value
 * @param update_func Behavior function
 * @return 0 on success, non-zero otherwise
 */
int(init_entity)(Entity *e, int16_t x, int16_t y, Sprite *sp, int16_t data, UpdateFunc update_func);

/**
 * @brief Reset an entity to inactive state
 *
 * @param e Entity to reset
 * @return 0 on success, non-zero otherwise
 */
int(reset_entity)(Entity *e);

/**
 * @brief Player update function
 *
 * Handles player movement, collision detection, and sprite updates
 */
void(update_player)(Entity *e, void *g, void *context);

/**
 * @brief Enemy update function
 *
 * Handles random enemy movement and collision detection
 */
void(update_enemy)(Entity *e, void *g, void *context);

/**@}*/

#endif /* __PROJ_ENTITY_H */
