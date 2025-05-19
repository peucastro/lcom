#ifndef __PROJ_SPRITE_H
#define __PROJ_SPRITE_H

#include "controller/graphics/graphics.h"

/** @defgroup sprite sprite
 * @{
 *
 * Functions for creating and manipulating sprites
 */

/**
 * @brief Struct representing a sprite.
 *
 * Contains the dimensions and pixel data for a sprite.
 */
typedef struct {
  uint16_t width, height; /**< @brief Width and height of the sprite in pixels */
  char *map;              /**< @brief Pointer to the sprite's pixel data */
} Sprite;

/**
 * @brief Creates a new sprite from an XPM image.
 *
 * Allocates memory for a new sprite and loads its pixel data from the provided XPM image.
 *
 * @param pic Array of strings containing the XPM image data
 * @return Pointer to the created sprite, or NULL if an error occurs
 */
Sprite *(create_sprite) (const char *pic[]);

/**
 * @brief Destroys a sprite and frees its resources.
 *
 * Frees the memory allocated for the sprite and its pixel data.
 *
 * @param sp Pointer to the sprite to be destroyed
 */
void(destroy_sprite)(Sprite *sp);

/**
 * @brief Renders a sprite to the screen at the specified position.
 *
 * Draws the sprite to the video buffer, skipping transparent pixels.
 *
 * @param sp Pointer to the sprite to be drawn
 * @param x The x-coordinate of the top-left corner where the sprite will be drawn
 * @param y The y-coordinate of the top-left corner where the sprite will be drawn
 * @return 0 upon success, non-zero otherwise
 */
int(draw_sprite)(Sprite *sp, int16_t x, int16_t y);

/**@}*/

#endif /* __PROJ_SPRITE_H */
