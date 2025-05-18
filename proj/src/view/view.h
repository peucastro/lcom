#ifndef __PROJ_VIEW_H
#define __PROJ_VIEW_H

#include "model/game/game.h"

/** @defgroup view view
 * @{
 *
 * Functions for rendering the game and its menus
 */

/**
 * @brief Draws the start menu
 *
 * Renders the start menu to the screen.
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_start_menu)(void);

/**
 * @brief Draws the pause menu
 *
 * Renders the pause menu to the screen.
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_pause_menu)(void);

/**
 * @brief Draws the game
 *
 * Renders the game board, player, enemies, bricks, walls, and bombs
 * to the screen.
 *
 * @param game Pointer to the game to be drawn
 * @return 0 upon success, non-zero otherwise
 */
int(draw_game)(Game *game);

/**
 * @brief Draws the next frame
 *
 * Determines the current game state and renders the appropriate
 * screen (start menu, pause menu, or game).
 *
 * @param game Pointer to the game instance
 */
void(draw_next_frame)(Game *game);

/**@}*/

#endif /* __PROJ_VIEW_H */
