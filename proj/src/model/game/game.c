#include <lcom/lcf.h>

#include "model/game/game.h"

extern Sprite *wall_sprite;
extern Sprite *brick_sprite;
extern Sprite *player_sprite;
extern Sprite *enemy_sprite;

int(init_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "init_game: game pointer cannot be null.");
    return 1;
  }

  game->state = START;

  game->board = create_board_from_file("/home/lcom/labs/proj/src/assets/boards/level1.txt");
  if (game->board == NULL) {
    fprintf(stderr, "init_game: failed to load game board.");
    return 1;
  }

  return 0;
}

void(draw_game)(Game *game) {
  graphics_clear_screen();

  if (game->state == START) {
    graphics_draw_rectangle(0, 0, 1152, 864, 0xFF0000);
  }
  else if (game->state == GAME) {
    graphics_draw_rectangle(0, 0, 1152, 864, 0x0000FF);

    if (game->board != NULL) {
      const int cell_size = 32;

      for (int row = 0; row < game->board->height; row++) {
        for (int col = 0; col < game->board->width; col++) {
          int x = col * cell_size;
          int y = row * cell_size;

          Sprite *sprite = NULL;

          switch (game->board->elements[row][col]) {
            case WALL:
              sprite = wall_sprite;
              break;
            case BRICK:
              sprite = brick_sprite;
              break;
            case PLAYER:
              sprite = player_sprite;
              break;
            case ENEMY:
              sprite = enemy_sprite;
              break;
            default:
              continue;
          }

          if (sprite != NULL) {
            sprite->x = x;
            sprite->y = y;
            draw_sprite(sprite);
          }
        }
      }
    }
  }
}
