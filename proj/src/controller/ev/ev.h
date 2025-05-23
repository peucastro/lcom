#ifndef __PROJ_EV_H
#define __PROJ_EV_H

#include "controller/kbc/mouse.h"
#include "model/game/game.h"

/** @defgroup ev ev
 * @{
 *
 * Module responsible for event handling in the game
 */

/**
 * @brief Handles timer events
 *
 * @param game Pointer to the game instance
 * @param counter The timer counter
 *
 * @return 0 upon success, non-zero otherwise
 */
int(handle_timer_event)(Game *game, uint32_t counter);

/**
 * @brief Handles keyboard events
 *
 * Processes a scancode event and updates the game state or player position accordingly.
 *
 * @param game Pointer to the game instance
 * @param scancode Scancode of the key pressed
 *
 * @return 0 upon success, non-zero otherwise
 */
int(handle_kbd_event)(Game *game, uint8_t scancode);

/**
 * @brief Handles mouse events
 *
 * Processes mouse movement and button events and updates the game accordingly
 * based on the current game state.
 *
 * @param game Pointer to the game instance
 * @param mouse_info Structure containing mouse position and button states
 *
 * @return 0 upon success, non-zero otherwise
 */
int(handle_mouse_event)(Game *game, mouse_info_t mouse_info);

/**@}*/

#endif /* __PROJ_EV_H */
