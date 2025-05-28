#ifndef __ANIM_SPRITE_H
#define __ANIM_SPRITE_H

#include "sprite.h"

typedef struct {
  Sprite *sp;
  Sprite *const *map;
  uint8_t num_fig;
  uint8_t cur_fig;
  uint16_t aspeed;
  uint16_t cur_aspeed;
  bool loop;
} AnimSprite;


AnimSprite *create_animSprite(Sprite *const *frames, uint8_t nframes, uint16_t ticks_per_frame, bool loop);

void destroy_animSprite(AnimSprite *asp);

void update_animSprite(AnimSprite *asp);

#endif /* __ANIM_SPRITE_H */
