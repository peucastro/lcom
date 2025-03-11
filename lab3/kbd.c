#include <lcom/lcf.h>

#include "i8042.h"
#include <lcom/lab3.h>

int hook_id = 5;

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  *bit_no = hook_id;
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int(kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}
