#include <lcom/lcf.h>

#include "mouse.h"

static int hook_id_mouse = 7;
static struct packet mouse_packet;
static uint8_t mouse_index = 0;
static uint8_t byte = 0;

struct packet *(mouse_get_packet) () {
  return &mouse_packet;
}

uint8_t(mouse_get_index)() {
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

void(mouse_sync)() {
  mouse_packet.bytes[mouse_index] = byte;

  if (byte & BIT(3)) {
    mouse_index = 0;
  }
  else {
    mouse_index = (mouse_index + 1) % 3;
  }
}

void(mouse_ih)(void) {
  kbc_read_data(&byte);
}
