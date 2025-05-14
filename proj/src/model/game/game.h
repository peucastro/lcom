#ifndef __PROJ_GAME_H
#define __PROJ_GAME_H

#include "controller/graphics/graphics.h"
#include "model/sprite/sprite.h"
#include "utils/board_parser.h"

/** @defgroup game game
 * @{
 *
 * Functions for managing the game state
 */

/**
 * @brief Enum representing the game state
 */
typedef enum {
  START,  /**< Game is in start menu */
  GAME,   /**< Game is being played */
  EXIT    /**< Game is exiting */
} game_state;

/**
 * @brief Struct representing the game
 */
typedef struct {
  game_state state; /**< Current state of the game */
  GameBoard *board; /**< Current game board */
} Game;

/**
 * @brief Initializes the game
 *
 * @param game Pointer to the game to be initialized
 * @return 0 upon success, non-zero otherwise
 */
int(init_game)(Game *game);

/**
 * @brief Draws the game
 *
 * @param game Pointer to the game to be drawn
 */
void(draw_game)(Game *game);

/**
 * @brief Handles an event
 *
 * @param game Pointer to the game
 * @param scancode Scancode of the event
 */
void(handle_event)(Game *game, uint8_t scancode);

/**@}*/

#endif /* __PROJ_GAME_H */
