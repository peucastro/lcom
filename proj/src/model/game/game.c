#include <lcom/lcf.h>

#include "model/game/game.h"
#include "model/resources/resources.h"

// External sprites from resources
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
  
  // Load the game board
  game->board = create_board_from_file("resources/boards/level1.txt");
  if (game->board == NULL) {
    fprintf(stderr, "init_game: failed to load game board.");
    return 1;
  }
  
  return 0;
}

void(draw_game)(Game *game) {
  graphics_clear_screen();
  
  if (game->state == START) {
    graphics_draw_rectangle(0, 0, 1152, 864, 0xFF0000); // Red background for start screen
    // Draw start screen text or menu here
  }
  else if (game->state == GAME) {
    graphics_draw_rectangle(0, 0, 1152, 864, 0x0000FF); // Blue background for game
    
    // Draw the game board if available
    if (game->board != NULL) {
      // Define the size of each cell in pixels
      const int cell_size = 32; // Adjust based on your sprite size
      
      for (int row = 0; row < game->board->height; row++) {
        for (int col = 0; col < game->board->width; col++) {
          // Calculate position
          int x = col * cell_size;
          int y = row * cell_size;
          
          // Select sprite based on board element
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
            // Add other cases as needed
            default:
              // No sprite for empty space
              continue;
          }
          
          if (sprite != NULL) {
            // Update sprite position
            sprite->x = x;
            sprite->y = y;
            // Draw the sprite
            draw_sprite(sprite);
          }
        }
      }
    }
  }
}

  // Add player movement handling here
