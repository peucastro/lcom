#include <lcom/lcf.h>

#include "mouse.h"

static int hook_id_mouse = 7;
static uint8_t mouse_packet[] = {0, 0, 0};
static uint8_t mouse_index = 0;

uint8_t *mouse_get_packet() {
  return mouse_packet;
}

uint8_t mouse_get_index() {
  return mouse_index;
}

int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1;
  }

  *bit_no = hook_id_mouse;

  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int(mouse_unsubscribe_int)(void) {
  return sys_irqrmpolicy(&hook_id_mouse);
}
