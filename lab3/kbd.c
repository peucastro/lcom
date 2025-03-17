#include <lcom/lcf.h>

#include "kbd.h"

int hook_id = 5;
uint8_t scancode = 0;

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  *bit_no = hook_id;
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int(kbd_unsubscribe_int)(void) {
  return sys_irqrmpolicy(&hook_id);
}

int(kbd_enable_int)(void) {
  uint8_t cmd;
  kbc_write_cmd(KBC_IN, KBC_READ_CMD);
  kbc_read_buffer(KBC_OUT, &cmd);
  cmd |= KBC_INT;
  kbc_write_cmd(KBC_IN, KBC_WRITE_CMD);
  kbc_write_cmd(KBC_ARG, cmd);
  return 0;
}

void(kbc_ih)(void) {
  kbc_read_buffer(KBC_OUT, &scancode);
}
