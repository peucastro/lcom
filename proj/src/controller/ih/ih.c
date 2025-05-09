#include <lcom/lcf.h>

#include "controller/ih/ih.h"

static uint32_t irq_set_timer, irq_set_kbd, irq_set_mouse;

int(subscribe_interrupts)(void) {
  uint8_t bit_no;

  if (timer_subscribe_int(&bit_no) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to subscribe timer interrupts.");
    return 1;
  }
  irq_set_timer = BIT(bit_no);

  if (kbd_subscribe_int(&bit_no) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to subscribe keyboard interrupts.");
    return 1;
  }
  irq_set_kbd = BIT(bit_no);

  if (mouse_write_cmd(MOUSE_EN_DATA_REPORTS) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to enable mouse data reporting.");
    return 1;
  }
  if (mouse_subscribe_int(&bit_no) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to subscribe mouse interrupts.");
    return 1;
  }
  irq_set_mouse = BIT(bit_no);

  return 0;
}

int(unsubscribe_interrupts)(void) {
  if (timer_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe timer interrupts.");
    return 1;
  }

  if (kbd_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe keyboard interrupts.");
    return 1;
  }

  if (mouse_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe mouse interrupts.");
    return 1;
  }
  if (mouse_write_cmd(MOUSE_DIS_DATA_REPORTS) != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to disable mouse data reporting.");
    return 1;
  }

  return 0;
}
