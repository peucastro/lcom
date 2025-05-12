#include <lcom/lcf.h>

#include "model/resources/resources.h"

Sprite *example;

int(create_resources)(void) {
  if ((example = create_sprite(example_xpm, 20, 20)) == NULL) {
    fprintf(stderr, "create_resources: failed to load example sprite.");
    return 1;
  }

  return 0;
}

void(destroy_resources)(void) {
  destroy_sprite(example);
}
