#ifndef __PROJ_BOARD_H
#define __PROJ_BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model/entity/entity.h"
#include "model/resources/resources.h"

/** @defgroup board_parser board_parser
 * @{
 *
 * Functions for parsing game board from text files
 */

#define MAX_LINE_LENGTH 256 /**< @brief Maximum line length for reading board files */
#define MAX_ENEMIES 32
#define MAX_BRICKS 256
#define MAX_WALLS 128

/**
 * @brief Enum representing different types of board elements
 */
typedef enum {
  EMPTY_SPACE, /**< Empty space where players can move */
  WALL,        /**< Indestructible wall */
  BRICK,       /**< Destructible brick */
  PLAYER,      /**< Player starting position */
  ENEMY,       /**< Enemy starting position */
  BOMB,        /**< Bomb */
  POWERUP      /**< Power-up item */
} BoardElement;

/**
 * @brief Struct representing the game board
 */
typedef struct {
  int width;               /**< Width of the board in cells */
  int height;              /**< Height of the board in cells */
  BoardElement **elements; /**< 2D array of board elements */
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

#endif /* __PROJ_BOARD_H */
