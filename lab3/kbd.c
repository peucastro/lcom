#include <lcom/lcf.h>

#include "kbd.h"

static int hook_id_kbd = 5;
uint8_t scancode = 0;

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    perror("bit_no is null.");
    return 1;
  }

  *bit_no = hook_id_kbd;
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd);
}

int(kbd_unsubscribe_int)(void) {
  return sys_irqrmpolicy(&hook_id_kbd);
}

int(kbd_enable_int)(void) {
  uint8_t cmd;
  if (kbc_write_cmd(KBC_IN, KBC_READ_CMD) != 0) {
    perror("Failed to write the KBC_READ_CMD.");
    return 1;
  }
  if (kbc_read_buffer(KBC_OUT, &cmd) != 0) {
    perror("Failed to read the kbc command.");
    return 1;
  }

  cmd |= KBC_INT;

  if (kbc_write_cmd(KBC_IN, KBC_WRITE_CMD) != 0) {
    perror("Failed to write the KBC_WRITE_CMD.");
    return 1;
  }
  if (kbc_write_cmd(KBC_ARG, cmd) != 0) {
    perror("Failed to pass the kbc argument.");
    return 1;
  }

  return 0;
}

void(kbc_ih)(void) {
  kbc_read_buffer(KBC_OUT, &scancode);
}
