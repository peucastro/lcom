#ifndef __PROJ_EV_H
#define __PROJ_EV_H

#include "model/game/game.h"

/** @defgroup ev ev
 * @{
 *
 * Module responsible for event handling in the game
 */

/**
 * @brief Handles keyboard events
 *
 * Processes a scancode event and updates the game state or player position accordingly.
 *
 * @param game Pointer to the game instance
 * @param scancode Scancode of the key pressed
 */
void(handle_kbd_event)(Game *game, uint8_t scancode);

/**@}*/

#endif /* __PROJ_EV_H */
