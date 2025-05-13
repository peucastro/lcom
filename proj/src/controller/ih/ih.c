#include <lcom/lcf.h>

#include "controller/ih/ih.h"

static uint32_t irq_set_timer = 0, irq_set_kbd = 0, irq_set_mouse = 0;
static uint8_t i = 0, bytes[2] = {0, 0};
static struct packet pp;

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
  if (mouse_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe mouse interrupts.");
    return 1;
  }
  if (mouse_write_cmd(MOUSE_DIS_DATA_REPORTS) != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to disable mouse data reporting.");
    return 1;
  }

  if (kbd_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe keyboard interrupts.");
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe timer interrupts.");
    return 1;
  }

  return 0;
}

void(timer_handler)(Game *game) {
  timer_int_handler();
  if (timer_get_counter() % sys_hz() == 0) {
    draw_game(game);
    timer_reset();
    // TODO: handle events related to the timer
  }
}

void(kbd_handler)(Game *game) {
  kbd_ih();
  bytes[i] = get_scancode();

  if (get_scancode() == CODE_HEADER) {
    i++;
    return;
  }

  i = 0;
  // TODO: handle events related to the kbd
  handle_event(game, get_scancode());
}

void(mouse_handler)(Game *game) {
  mouse_ih();
  mouse_sync();

  if (mouse_get_index() == 0) {
    pp = mouse_parse_packet();
    // TODO: handle events related to the mouse
  }
}

void(process_interrupts)(uint32_t irq_mask, Game *game) {
  if (irq_mask & irq_set_timer) {
    timer_handler(game);
  }
  if (irq_mask & irq_set_kbd) {
    kbd_handler(game);
  }
  if (irq_mask & irq_set_mouse) {
    mouse_handler(game);
  }
}
