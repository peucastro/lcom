#ifndef __PROJ_ENTITY_H
#define __PROJ_ENTITY_H

#include "model/sprite/sprite.h"

/**
 * @brief Struct representing an entity in the game
 *
 * An entity represents a game object with a position, a sprite, and a status
 * indicating whether it is alive or not.
 */
typedef struct {
  int16_t x, y;   /**< @brief x and y coordinates of the entity's position */
  Sprite *sprite; /**< @brief The entity's corresponding sprite */
  bool alive;     /**< @brief Indicates whether the entity is alive */
} Entity;

/**
 * @brief Creates a new entity
 *
 * @param x The x-coordinate of the entity's position
 * @param y The y-coordinate of the entity's position
 * @param sp Pointer to the sprite associated with the entity
 * @return Pointer to the created entity, or NULL if an error occurs
 */
Entity *(create_entity) (int16_t x, int16_t y, Sprite *sp);

/**
 * @brief Destroys an entity and frees its resources
 *
 * @param entity Pointer to the entity to be destroyed
 */
void(destroy_entity)(Entity *entity);

#endif /* __PROJ_ENTITY_H */
