#include <lcom/lcf.h>

#include "model/resources/resources.h"

Sprite *example;

int(create_resources)(void) {
  example = create_sprite(example_xpm, 20, 20);

  return 0;
}

void(destroy_resources)(void) {
  destroy_sprite(example);
}
