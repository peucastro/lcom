#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include <lcom/lcf.h>
#include "game.h"

void move_player(Entity *p, Game *game, int16_t xmov, int16_t ymov);
void schedule_enemy_moves(Game *game);

#endif // _MOVEMENT_H_
