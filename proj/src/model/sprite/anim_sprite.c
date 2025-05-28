#include <stdlib.h>
#include "sprite.h"
#include "anim_sprite.h"

AnimSprite *create_animSprite(Sprite *const *frames, uint8_t nframes, uint16_t ticks_per_frame, bool loop) {
  AnimSprite *a = malloc(sizeof(*a));
  if (!a) return NULL;

  a->map = frames;
  a->num_fig = nframes;
  a->aspeed = ticks_per_frame;
  a->cur_aspeed = 0;
  a->cur_fig = 0;
  a->loop = loop;
  a->sp = frames[0];

  return a;
}

void destroy_animSprite(AnimSprite *asp) {
  free(asp);
}

void update_animSprite(AnimSprite *asp) {
  if (!asp) return;
  if (++asp->cur_aspeed < asp->aspeed) return;

  asp->cur_aspeed = 0;
  if (asp->loop)  asp->cur_fig = (asp->cur_fig + 1) % asp->num_fig;
  else if (asp->cur_fig + 1 < asp->num_fig) asp->cur_fig++;

  asp->sp = asp->map[asp->cur_fig];
}
