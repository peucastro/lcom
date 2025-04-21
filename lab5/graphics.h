#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>

#include "vbe.h"

/** @defgroup graphics graphics
 * @{
 *
 * Functions for using the PC's video card in Graphics Mode.
 */

/**
 * @brief Set default MINIX text mode
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_exit)(void);

/**
 * @brief Initializes the video card in graphics mode
 *
 * @param mode 16-bit VBE mode to set
 * @return Return 0 upon sucess, non-zero otherwise
 */
int(graphics_set_video_mode)(uint16_t mode);

/**
 * @brief Maps the video card VRAM to the program's address space
 *
 * @param mode 16-bit VBE mode to set
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_map_vram)(uint16_t mode);

/**
 * @brief Draws a pixel in the screen
 *
 * @param x Horizontal axis coordinate
 * @param y Vertial axis coordinate
 * @param color Color to set the pixel
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line on the screen
 *
 * @param x Horizontal axis coordinate of the initial point
 * @param y Vertical axis coordinate of the initial point
 * @param len Line's length in pixels
 * @param color Color to draw the line's pixels
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a filled rectangle on the screen
 *
 * @param x Horizontal axis coordinate of the initial point
 * @param y Vertical axis coordinate of the initial point
 * @param width Rectangle's width in pixels
 * @param height Rectangle's height in pixels
 * @param color Color to set the rectangle's pixels
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

#endif
