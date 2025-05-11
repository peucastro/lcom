#ifndef __PROJ_GAME_H
#define __PROJ_GAME_H

#include "controller/graphics/graphics.h"

/**
 * @brief Represents the different states the game can be in during execution.
 */
typedef enum game_state {
  START, /**< @brief Initial state when the game is first launched */
  GAME,  /**< @brief Active gameplay state */
  EXIT   /**< @brief Final state when the game is ending */
} game_state;

typedef struct {
  game_state state;
} Game;

void(init_game)(Game *game);

void(draw_game)(Game *game);

#endif /* __PROJ_GAME_H */
