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

int(kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void(kbc_ih)() {
  uint8_t st;

  read_kbc_st(&st);
  if (!kbc_ready(&st))
    return;

  read_kbc_data(&scancode);
}
