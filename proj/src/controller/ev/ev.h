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

int(handle_mouse_event)(Game *game, mouse_info_t mouse_info);

/**@}*/

#endif /* __PROJ_EV_H */
