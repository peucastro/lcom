#ifndef __PROJ_INPUT_H
#define __PROJ_INPUT_H

#include "controller/kbc/mouse.h"
#include "model/game/game.h"

/** @defgroup input input
 * @{
 *
 * Functions for handling user input (keyboard and mouse)
 */

/**
 * @brief Key enumeration for supported input keys
 */
typedef enum {
  KEY_NONE,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ENTER,
  KEY_ESCAPE,
} Key;

/* button hit rectangles for menu navigation */
#define START_BX 243 /**< @brief Left edge of "Start" button */
#define START_BY 541 /**< @brief Top of both buttons */
#define BUTTON_W 230 /**< @brief Width of "Start" button */
#define BUTTON_H 35  /**< @brief Height of both buttons */

#define EXIT_BX 617 /**< @brief Left edge of "Exit" button */
#define EXIT_BY 541 /**< @brief Same top Y as "Start" */

/**
 * @brief Translates a scancode to a Key enum value
 *
 * @param scancode The scancode to translate
 *
 * @return The corresponding Key enum value, or KEY_NONE if not recognized
 */
Key(translate_scancode)(uint8_t scancode);

/**
 * @brief Handles keyboard input based on current game state
 *
 * Processes keyboard input and updates the game state accordingly.
 * Handles both menu navigation and in-game controls.
 *
 * @param game Pointer to the game instance
 * @param key The key that was pressed
 */
void(handle_keyboard_input)(Game *game, Key key);

/**
 * @brief Handles mouse input based on current game state
 *
 * Processes mouse movement and button clicks.
 * Updates menu selection and handles game interactions.
 *
 * @param game Pointer to the game instance
 */
void(handle_mouse_input)(Game *game);

/**@}*/

#endif /* __PROJ_INPUT_H */
