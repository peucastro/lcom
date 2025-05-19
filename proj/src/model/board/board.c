#include <lcom/lcf.h>

#include "model/board/board.h"

/**
 * @brief Converts a character from the board file to a board_element_t
 *
 * @param c Character to convert
 * @return Corresponding board_element_t
 */
static board_element_t(char_to_element)(char c) {
  switch (c) {
    case 'P': return PLAYER;
    case 'E': return ENEMY;
    case 'B': return BRICK;
    case '#': return WALL;
    case 'O': return BOMB;
    case 'U': return POWERUP;
    case ' ': return EMPTY_SPACE;
    default: return EMPTY_SPACE;
  }
}

GameBoard *(create_board) (const char *filename) {
  if (filename == NULL) {
    fprintf(stderr, "create_board: filename cannot be null");
    return NULL;
  }

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "create_board: failed to open file %s", filename);
    return NULL;
  }

  size_t width = 0;
  size_t height = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    size_t len = strlen(line);
    if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      line[--len] = '\0';
    }
    if (len > 0) {
      if (width == 0) {
        width = len;
      }
      else if (len != width) {
        fprintf(stderr, "create_board: inconsistent line lengths in file %s", filename);
        if (fclose(file) != 0) {
          fprintf(stderr, "create_board: failed to close file %s after error", filename);
        }
        return NULL;
      }
      height++;
    }
  }

  if (width == 0 || height == 0) {
    fprintf(stderr, "create_board: empty or invalid board file %s", filename);
    if (fclose(file) != 0) {
      fprintf(stderr, "create_board: failed to close file %s after error", filename);
    }
    return NULL;
  }

  GameBoard *board = (GameBoard *) malloc(sizeof(GameBoard));
  if (board == NULL) {
    fprintf(stderr, "create_board: failed to allocate memory for board");
    if (fclose(file) != 0) {
      fprintf(stderr, "create_board: failed to close file %s after error", filename);
    }
    return NULL;
  }

  board->width = width;
  board->height = height;

  board->elements = (board_element_t **) malloc(height * sizeof(board_element_t *));
  if (board->elements == NULL) {
    fprintf(stderr, "create_board: failed to allocate memory for board elements");
    free(board);
    board = NULL;
    if (fclose(file) != 0) {
      fprintf(stderr, "create_board: failed to close file %s after error", filename);
    }
    return NULL;
  }

  for (uint8_t i = 0; i < height; i++) {
    board->elements[i] = (board_element_t *) malloc(width * sizeof(board_element_t));
    if (board->elements[i] == NULL) {
      fprintf(stderr, "create_board: failed to allocate memory for board row %d", i);
      for (uint8_t j = 0; j < i; j++) {
        free(board->elements[j]);
        board->elements[j] = NULL;
      }
      free(board->elements);
      board->elements = NULL;
      free(board);
      board = NULL;
      if (fclose(file) != 0) {
        fprintf(stderr, "create_board: failed to close file %s after error", filename);
      }
      return NULL;
    }
  }

  rewind(file);
  uint8_t row = 0;

  while (fgets(line, MAX_LINE_LENGTH, file) != NULL && row < height) {
    size_t len = strlen(line);
    if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      line[--len] = '\0';
    }
    if (len > 0) {
      for (uint8_t col = 0; col < width; col++) {
        board->elements[row][col] = char_to_element(line[col]);
      }
      row++;
    }
  }

  if (fclose(file) != 0) {
    fprintf(stderr, "create_board: failed to close file %s after reading", filename);
  }

  return board;
}

int(destroy_board)(GameBoard *board) {
  if (board == NULL) {
    fprintf(stderr, "destroy_board: board pointer cannot be null");
    return 1;
  }

  if (board->elements != NULL) {
    for (uint8_t i = 0; i < board->height; i++) {
      if (board->elements[i] != NULL) {
        free(board->elements[i]);
        board->elements[i] = NULL;
      }
    }
    free(board->elements);
    board->elements = NULL;
  }

  free(board);
  board = NULL;

  return 0;
}
