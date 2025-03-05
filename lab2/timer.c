#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;
int counter = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2 || freq < 19 || freq > TIMER_FREQ)
    return 1;

  uint8_t ctrl_word;
  if (timer_get_conf(timer, &ctrl_word) != 0)
    return 1;

  ctrl_word &= 0x0F;
  ctrl_word = ctrl_word | TIMER_LSB_MSB;
  switch (timer) {
    case 0:
      ctrl_word |= TIMER_SEL0;
      break;
    case 1:
      ctrl_word |= TIMER_SEL1;
      break;
    case 2:
      ctrl_word |= TIMER_SEL2;
      break;
    default:
      return 1;
  }

  uint16_t count = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  if (util_get_LSB(count, &lsb) != 0)
    return 1;
  if (util_get_MSB(count, &msb) != 0)
    return 1;

  if (sys_outb(TIMER_CTRL, ctrl_word) != 0)
    return 1;
  if (sys_outb(TIMER_0 + timer, lsb) != 0)
    return 1;
  if (sys_outb(TIMER_0 + timer, msb) != 0)
    return 1;

  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id);

  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int(timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void(timer_int_handler)() {
  counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer < 0 || timer > 2 || st == NULL)
    return 1;

  uint8_t rdb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if ((sys_outb(TIMER_CTRL, rdb_cmd)) != 0)
    return 1;

  if ((util_sys_inb(TIMER_0 + timer, st)) != 0)
    return 1;

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  if (timer < 0 || timer > 2)
    return 1;

  union timer_status_field_val val;

  switch (field) {
    case tsf_all:
      val.byte = st;
      break;

    case tsf_initial:
      st = ((st >> 4) & 0x3);
      if (st == 1) {
        val.in_mode = LSB_only;
      }
      else if (st == 2) {
        val.in_mode = MSB_only;
      }
      else if (st == 3) {
        val.in_mode = MSB_after_LSB;
      }
      else {
        val.in_mode = INVAL_val;
      }
      break;

    case tsf_mode:
      st = ((st >> 1) & 0x7);
      if (st == 6) {
        val.count_mode = 2;
      }
      else if (st == 7) {
        val.count_mode = 3;
      }
      else {
        val.count_mode = st;
      }
      break;

    case tsf_base:
      val.bcd = (st & TIMER_BCD);
      break;

    default:
      return 1;
  }

  if ((timer_print_config(timer, field, val)) != 0)
    return 1;

  return 0;
}
