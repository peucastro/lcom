#ifndef __PROJ_BOARD_H
#define __PROJ_BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model/entity/entity.h"

/** @defgroup board board
 * @{
 *
 * Functions for parsing game board from text files
 */

#define MAX_LINE_LENGTH 64  /**< @brief Maximum line length for reading board files */
#define MAX_BOARD_WIDTH 16  /**< @brief Maximum width of the game board */
#define MAX_BOARD_HEIGHT 11 /**< @brief Maximum height of the game board */

/**
 * @brief Enum representing different types of board elements
 */
typedef enum {
  PLAYER,     /**< @brief Player starting position */
  ENEMY,      /**< @brief Enemy starting position */
  BRICK,      /**< @brief Destructible brick */
  WALL,       /**< @brief Indestructible wall */
  BOMB,       /**< @brief Bomb */
  POWERUP,    /**< @brief Power-up item */
  EMPTY_SPACE /**< @brief Empty space where players can move */
} board_element_t;

/**
 * @brief Struct representing the game board
 */
typedef struct {
  uint8_t width;                                               /**< @brief Width of the board in cells */
  uint8_t height;                                              /**< @brief Height of the board in cells */
  board_element_t elements[MAX_BOARD_HEIGHT][MAX_BOARD_WIDTH]; /**< @brief 2D array of board elements */
} GameBoard;

/**
 * @brief Creates a new game board from a text file
 *
 * @param board Pointer to the GameBoard structure to fill
 * @param filename Path to the text file containing the board layout
 *
 * @return 0 upon success, non-zero otherwise
 */
int(load_board)(GameBoard *board, const char *filename);

/**
 * @brief Resets a game board
 *
 * @param board Pointer to the board to be reset
 *
 * @return 0 upon success, non-zero otherwise
 */
int(reset_board)(GameBoard *board);

/**@}*/

#endif /* __PROJ_BOARD_H */
