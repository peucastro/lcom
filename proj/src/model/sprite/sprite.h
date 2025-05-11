#ifndef __PROJ_SPRITE_H
#define __PROJ_SPRITE_H

#include "controller/graphics/graphics.h"

/** @defgroup sprite sprite
 * @{
 *
 * Functions for creating and manipulating sprites
 */

/**
 * @brief Struct representing a sprite
 *
 * Contains the position, dimensions, and pixel data for a sprite
 */
typedef struct {
  int16_t x, y;           /**< @brief x and y coordinates of the sprite's position */
  uint16_t width, height; /**< @brief width and height of the sprite in pixels */
  char *map;              /**< @brief pointer to the sprite's pixel data */
} Sprite;

/**
 * @brief Creates a new sprite from an XPM image
 *
 * @param pic Array of strings containing the XPM image data
 * @param x Initial x coordinate for the sprite
 * @param y Initial y coordinate for the sprite
 * @return Pointer to the created sprite, or NULL if an error occurs
 */
Sprite *(create_sprite) (const char *pic[], int16_t x, int16_t y);

/**
 * @brief Destroys a sprite and frees its resources
 *
 * @param sp Pointer to the sprite to be destroyed
 */
void(destroy_sprite)(Sprite *sp);

/**@}*/

#endif /* __PROJ_SPRITE */
