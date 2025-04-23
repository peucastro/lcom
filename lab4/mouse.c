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
    perror("mouse_subscribe_int: bit_no cannot be null.");
    return 1;
  }

  *bit_no = hook_id_mouse;

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != 0) {
    perror("mouse_subscribe_int: failed to set the mouse interrupt subscription policy.");
    return 1;
  }

  return 0;
}

int(mouse_unsubscribe_int)(void) {
  if (sys_irqrmpolicy(&hook_id_mouse) != 0) {
    perror("mouse_unsubscribe_int: failed to set the mouse interrupt subscription policy.");
    return 1;
  }

  return 0;
}

int(mouse_write_cmd)(uint8_t cmd) {
  int attempts = KBC_MAX_ATTEMPTS;
  uint8_t response;

  while (attempts > 0) {
    // request forwarding of the byte (command) to the mouse
    if (kbc_write_cmd(KBC_IN, MOUSE_WRITE_BYTE) != 0) { // write 0xD4 to port 0x64
      perror("mouse_write_cmd: failed to write MOUSE_WRITE_BYTE to the kbc.");
      return 1;
    }

    // write the actual mouse command to port 0x60
    if (kbc_write_cmd(KBC_WRITE_CMD, cmd) != 0) { // write the command to port 0x60
      perror("mouse_write_cmd: failed to write the mouse command to the kbc.");
      return 1;
    }

    // wait for the acknowledgment byte from the mouse
    micro_delay(micros_to_ticks(20000)); // delay to allow the mouse to process the command

    if (util_sys_inb(KBC_OUT, &response) != 0) { // read the acknowledgment byte from port 0x60
      perror("mouse_write_cmd: failed to read the kbc response.");
      return 1;
    }

    // check the acknowledgment byte
    if (response == MOUSE_NACK) { // 0xFE: command was not acknowledged, retry
      perror("mouse_write_cmd: mouse NACK");
      return 1;
    }
    if (response == MOUSE_ERR) { // 0xFC: command resulted in an error
      perror("mouse_write_cmd: mouse ERROR");
      return 1;
    }
    if (response == MOUSE_ACK) { // 0xFA: command was successfully acknowledged
      return 0;
    }

    // decrement the number of attempts left
    attempts--;
  }

  perror("mouse_write_cmd: failed to write the mouse command.");
  return 1;
}

void(mouse_sync)(void) {
  // check if the driver is expecting the first byte of a packet
  if ((mouse_index == 0) && !(byte & BIT(3))) {
    /* if bit 3 of the received byte is 0, the driver is not in sync.
     * ignore the byte and return */
    return;
  }
  else {
    // store the received byte in the packet buffer
    mouse_packet_bytes[mouse_index] = byte;

    /* update the index to track the next byte.
     * use modular arithmetic to cycle the index through 0, 1, and 2 */
    mouse_index = (mouse_index + 1) % 3;
  }
}

struct packet(mouse_parse_packet)(void) {
  struct packet p;

  if (mouse_index != 0) {
    perror("mouse_parse_packet: couldn't assemble the mouse packet");
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
