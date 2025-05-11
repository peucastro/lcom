#ifndef __PROJ_GAME_H
#define __PROJ_GAME_H

/**
 * @brief Represents the different states the game can be in during execution.
 */
typedef enum game_state {
  START, /**< @brief Initial state when the game is first launched */
  GAME,  /**< @brief Active gameplay state */
  EXIT   /**< @brief Final state when the game is ending */
} game_state;

game_state(get_game_state)(void);

void(update_game_state)(game_state new_state);

#endif
