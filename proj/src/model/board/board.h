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

#define MAX_LINE_LENGTH 64 /**< @brief Maximum line length for reading board files */

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
} BoardElement;

/**
 * @brief Struct representing the game board
 */
typedef struct {
  int width;               /**< @brief Width of the board in cells */
  int height;              /**< @brief Height of the board in cells */
  BoardElement **elements; /**< @brief 2D array of board elements */
} GameBoard;

/**
 * @brief Creates a new game board from a text file
 *
 * @param filename Path to the text file containing the board layout
 * @return Pointer to the created game board, or NULL if an error occurs
 */
GameBoard *(create_board_from_file) (const char *filename);

/**
 * @brief Destroys a game board and frees its resources
 *
 * @param board Pointer to the board to be destroyed
 */
void(destroy_board)(GameBoard *board);

/**@}*/

#endif /* __PROJ_BOARD_H */
