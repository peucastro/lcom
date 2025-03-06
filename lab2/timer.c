#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

/*
 * bit position to activate in the message for the interrupt notifications
 * it tells us which bit in msg.m_notify.interrupts corresponds to timer 0 interrupts
 * the value is arbitrary (but should be between 0 and 31)
 */
int hook_id = 3;
int counter = 0; // global counter (in seconds)

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  // MINIX does not support frequencies below 19 (it causes overflow). TIMER_FREQ / 2^16 = 18.2
  if (timer < 0 || timer > 2 || freq < 19 || freq > TIMER_FREQ)
    return 1;

  uint8_t ctrl_word;
  if (timer_get_conf(timer, &ctrl_word) != 0) // get the currect config
    return 1;

  /*
   * before we proceed, we need to clear out the first for bits of the timer's config!
   * the 4 most significant bits of the status represent the output, null count and type of access,
   * and we need to write the counter selection and initialization mode in order to change the config
   */
  ctrl_word &= 0x0F;                     // clear the first 4 bits
  ctrl_word = ctrl_word | TIMER_LSB_MSB; // set the initialization mode to LSB followed by MSB

  // set the counter selection bits based on the timer number
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

  uint16_t count = TIMER_FREQ / freq; // calculate the internal value
  uint8_t lsb, msb;
  if (util_get_LSB(count, &lsb) != 0) // get the LSB of the count
    return 1;
  if (util_get_MSB(count, &msb) != 0) // get the MSB of the count
    return 1;

  if (sys_outb(TIMER_CTRL, ctrl_word) != 0) // tell the timer we are going to change its config
    return 1;
  if (sys_outb(TIMER_0 + timer, lsb) != 0) // write the LSB
    return 1;
  if (sys_outb(TIMER_0 + timer, msb) != 0) // followed by the MSB
    return 1;

  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = hook_id; // sets the bit_no to be the bit position in the mask returned upon an interrupt

  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id); // subscribes the interrupt notification
}

int(timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id); // unsubscribes the notification
}

void(timer_int_handler)() {
  counter++; // increases the counter
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer < 0 || timer > 2 || st == NULL)
    return 1;

  /*
   * reading a timer's configuration requires a special command: the Read-Back command.
   * TIMER_RB_CMD: distinguish between a control word and a read-back command.
   * TIMER_RB_COUNT_: we want to read the config, so we need to activate the status bit
   * TIMER_RB_SEL: selects the timer
   */
  uint8_t rdb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if ((sys_outb(TIMER_CTRL, rdb_cmd)) != 0) // writes the rdb_cmd to the control register
    return 1;

  // TIMER_0 [0x40] + timer [0, 1 or 2]: specifies the desired port
  if ((util_sys_inb(TIMER_0 + timer, st)) != 0) // efectivelly reads the timer's configuration
    return 1;

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer < 0 || timer > 2)
    return 1;

  union timer_status_field_val val; // union to store the value of the specified status field

  switch (field) { // switch based on the specified field to display
    case tsf_all:
      val.byte = st; // display the entire status byte
      break;

    case tsf_initial:
      st = ((st >> 4) & 0x3); // extract the initialization mode (bits 4 and 5)
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
      st = ((st >> 1) & 0x7); // extract the counting mode (bits 1, 2, and 3)
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
      val.bcd = (st & TIMER_BCD); // extract the counting base (bit 0)
      break;

    default:
      return 1;
  }

  if ((timer_print_config(timer, field, val)) != 0) // print the timer configuration
    return 1;

  return 0;
}
