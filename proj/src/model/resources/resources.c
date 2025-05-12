#include <lcom/lcf.h>

#include "model/resources/resources.h"

Sprite *niaefeup;

int(create_resources)(void) {
  niaefeup = create_sprite(niaefeup_xpm, 20, 20);

  return 0;
}

void(destroy_resources)(void) {
  destroy_sprite(niaefeup);
}
