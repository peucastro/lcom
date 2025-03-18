#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"
#include "kbd.h"

extern uint8_t scancode;
uint8_t bytes[2];
extern int cnt_sys_inb;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status, r, size = 0;
  uint8_t bit_no, irq_set;
  message msg;

  if (kbd_subscribe_int(&bit_no) != 0)
    return 1;
  irq_set = BIT(bit_no); // create a bitmask to "filter" the interrupt messages

  while (scancode != BREAK_ESC) { /*breakcode ESC*/
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih();
            if (scancode == CODE_HEADER) {
              bytes[0] = scancode;
              size++;
            }
            else {
              kbc_ih();
              bytes[size] = scancode;
            }

            kbd_print_scancode(!(scancode & MAKE_CODE), size + 1, bytes);
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
    size = 0;
  }

  if (kbd_unsubscribe_int() != 0)
    return 1;

  if (kbd_print_no_sysinb(cnt_sys_inb) != 0)
    return 1;

  return 0;
}

int(kbd_test_poll)() {
  int size = 0;
  while (scancode != BREAK_ESC) {
    if (kbc_read_data(&scancode) == 0) {
      if (scancode == CODE_HEADER) {
        bytes[0] = scancode;
        size++;
      }
      else {
        kbc_read_data(&scancode);
        bytes[size] = scancode;
      }

      kbd_print_scancode(!(scancode & MAKE_CODE), size + 1, bytes);
      size = 0;
    }
    tickdelay(micros_to_ticks(20000));
  }

  if (kbd_enable_int() != 0)
    return 1;

  if (kbd_print_no_sysinb(cnt_sys_inb) != 0)
    return 1;

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, r, size = 0;
  uint8_t bit_no, irq_set_timer, irq_set_kbd, time = 0;
  message msg;

  if (timer_subscribe_int(&bit_no) != 0)
    return 1;
  irq_set_timer = BIT(bit_no);

  if (kbd_subscribe_int(&bit_no) != 0)
    return 1;
  irq_set_kbd = BIT(bit_no);

  while (scancode != BREAK_ESC && time < n) { /* breakcode ESC and timeout check */
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_kbd) { /* subscribed interrupt */
            kbc_ih();
            if (scancode == CODE_HEADER) {
              bytes[0] = scancode;
              size++;
            }
            else {
              kbc_ih();
              bytes[size] = scancode;
            }

            counter = 0;
            time = 0;
            kbd_print_scancode(!(scancode & MAKE_CODE), size + 1, bytes);
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (counter % 60 == 0) {
              time++;
            }
          }
            break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
    size = 0;
  }

  if (kbd_unsubscribe_int() != 0)
    return 1;
  if(timer_unsubscribe_int() != 0)
    return 1;

  if (kbd_print_no_sysinb(cnt_sys_inb) != 0)
    return 1;

  return 0;
}
