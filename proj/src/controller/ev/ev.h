#ifndef __PROJ_EV_H
#define __PROJ_EV_H

#include "model/game/game.h"

/** @defgroup ev ev
 * @{
 *
 * Module responsible for event handling in the game
 */

void(handle_event)(Game *game, uint8_t scancode);

/**@}*/

#endif /* __PROJ_EV_H */
