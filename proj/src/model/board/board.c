#include "model/board/board.h"

/**
 * @brief Converts a character from the board file to a BoardElement
 *
 * @param c Character to convert
 * @return Corresponding BoardElement
 */
static BoardElement char_to_element(char c) {
  switch (c) {
    case ' ': return EMPTY_SPACE;
    case '#': return WALL;
    case 'B': return BRICK;
    case 'P': return PLAYER;
    case 'E': return ENEMY;
    case 'O': return BOMB;
    case 'U': return POWERUP;
    default: return EMPTY_SPACE;
  }
}

GameBoard *(create_board_from_file) (const char *filename) {
  if (filename == NULL) {
    fprintf(stderr, "create_board_from_file: filename cannot be null.\n");
    return NULL;
  }

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "create_board_from_file: failed to open file %s.\n", filename);
    return NULL;
  }

  int width = 0;
  int height = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    size_t len = strlen(line);
    if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      line[--len] = '\0';
    }
    if (len > 0) {
      if (width == 0) {
        width = (int) len;
      }
      else if ((int) len != width) {
        fprintf(stderr, "create_board_from_file: inconsistent line lengths in file %s.\n", filename);
        fclose(file);
        return NULL;
      }
      height++;
    }
  }

  if (width == 0 || height == 0) {
    fprintf(stderr, "create_board_from_file: empty or invalid board file %s.\n", filename);
    fclose(file);
    return NULL;
  }

  GameBoard *board = (GameBoard *) malloc(sizeof(GameBoard));
  if (board == NULL) {
    fprintf(stderr, "create_board_from_file: failed to allocate memory for board.\n");
    fclose(file);
    return NULL;
  }

  board->width = width;
  board->height = height;

  board->elements = (BoardElement **) malloc(height * sizeof(BoardElement *));
  if (board->elements == NULL) {
    fprintf(stderr, "create_board_from_file: failed to allocate memory for board elements.\n");
    free(board);
    fclose(file);
    return NULL;
  }

  Entity *player = NULL;
  Entity **enemies = malloc(sizeof(Entity *) * MAX_ENEMIES);
  Entity **bricks = malloc(sizeof(Entity *) * MAX_BRICKS);
  Entity **walls = malloc(sizeof(Entity *) * MAX_WALLS);
  int enemy_count = 0, brick_count = 0, wall_count = 0;

  for (int i = 0; i < height; i++) {
    board->elements[i] = (BoardElement *) malloc(width * sizeof(BoardElement));
    if (board->elements[i] == NULL) {
      fprintf(stderr, "create_board_from_file: failed to allocate memory for board row %d.\n", i);
      for (int j = 0; j < i; j++) {
        free(board->elements[j]);
      }
      free(board->elements);
      free(board);
      fclose(file);
      return NULL;
    }
  }

  const Resources *resources = get_resources();

  rewind(file);
  int row = 0;

  while (fgets(line, MAX_LINE_LENGTH, file) != NULL && row < height) {
    size_t len = strlen(line);
    if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      line[--len] = '\0';
    }
    if (len > 0) {
      for (int col = 0; col < width && col < (int) len; col++) {
        char ch = line[col];
        BoardElement elem = char_to_element(ch);

        switch (elem) {
          case PLAYER:
            player = create_entity(col, row, resources->player_sprite);
            board->elements[row][col] = PLAYER;
            break;
          case ENEMY:
            if (enemy_count < MAX_ENEMIES) {
              enemies[enemy_count++] = create_entity(col, row, resources->enemy_sprite);
            }
            board->elements[row][col] = ENEMY;
            break;
          case BRICK:
            if (brick_count < MAX_BRICKS) {
              bricks[brick_count++] = create_entity(col, row, resources->brick_sprite);
            }
            board->elements[row][col] = BRICK;
            break;
          case WALL:
            if (wall_count < MAX_WALLS) {
              walls[wall_count++] = create_entity(col, row, resources->wall_sprite);
            }
            board->elements[row][col] = WALL;
            break;
          default:
            board->elements[row][col] = EMPTY_SPACE;
        }
      }
      row++;
    }
  }

  fclose(file);
  return board;
}

void(destroy_board)(GameBoard *board) {
  if (board == NULL) {
    return;
  }

  if (board->elements != NULL) {
    for (int i = 0; i < board->height; i++) {
      if (board->elements[i] != NULL) {
        free(board->elements[i]);
      }
    }
    free(board->elements);
  }

  free(board);
}
