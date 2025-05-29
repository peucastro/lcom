#ifndef _COMBAT_H_
#define _COMBAT_H_

#include <lcom/lcf.h>
#include "game.h"

#define EXPLOSION_LIFETIME_TICKS (EXPLOSION_ANIM_FRAMES * EXPLOSION_ANIM_SPEED)

void drop_bomb(Game *game, int16_t x, int16_t y);
void explode_bomb(Game *game, uint8_t bomb_index);
void update_bombs(Game *game);
void update_explosions(Game *game);

#endif // _COMBAT_H_
