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

int(mouse_write_cmd)(uint8_t cmd) {
  int attempts = 10;
  uint8_t response;

  do {
    attempts--;
    if (kbc_write_cmd(KBC_IN, MOUSE_WRITE_BYTE) != 0)
      return 1;
    if (kbc_write_cmd(KBC_WRITE_CMD, cmd) != 0)
      return 1;
    micro_delay(micros_to_ticks(20000));
    if (kbc_read_buffer(KBC_OUT, &response) != 0)
      return 1;
    if (response == MOUSE_ACK)
      return 0;
  } while (attempts > 0);

  return 1;
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
