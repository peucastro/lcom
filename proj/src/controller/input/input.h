#ifndef __INPUT_H
#define __INPUT_H

#include <stdint.h>

typedef enum {
  KEY_NONE,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ENTER,
  KEY_ESCAPE,
} Key;

Key translate_scancode(uint8_t scancode);

#endif
