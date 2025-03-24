#include <lcom/lcf.h>

#include "mouse.h"

static int hook_id_mouse = 7;
static uint8_t mouse_packet_bytes[3] = {0, 0, 0};
static uint8_t mouse_index = 0;
static uint8_t byte = 0;

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
  mouse_packet_bytes[mouse_index] = byte;

  if (byte & BIT(3)) {
    mouse_index = 0;
  }
  else {
    mouse_index = (mouse_index + 1) % 3;
  }
}

struct packet(mouse_parse_packet)(void) {
  struct packet p;

  if (mouse_index != 0) {
    perror("couldn't assemble the mouse packet");
    return p;
  }

  for (int i = 0; i < 3; i++) {
    p.bytes[i] = mouse_packet_bytes[i];
  }

  p.rb = p.bytes[0] & MOUSE_RB;
  p.mb = p.bytes[0] & MOUSE_MB;
  p.lb = p.bytes[0] & MOUSE_LB;

  p.delta_x = (p.bytes[0] & MOUSE_XSIGN) ? (0xFF00 | p.bytes[1]) : (p.bytes[1]);
  p.delta_y = (p.bytes[0] & MOUSE_YSIGN) ? (0xFF00 | p.bytes[2]) : (p.bytes[2]);

  p.x_ov = p.bytes[0] & MOUSE_XOV;
  p.y_ov = p.bytes[0] & MOUSE_YOV;

  return p;
}

void(mouse_ih)(void) {
  kbc_read_data(&byte);
}
