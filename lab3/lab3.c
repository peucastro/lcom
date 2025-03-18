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

  if (kbd_subscribe_int(&bit_no) != 0) { // subscribes for the kbd interrupts
    perror("Failed to subscribe kbd interrupts.");
    return 1;
  }
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
            kbc_ih();                              // calls the interrupt handler

            /*
             * scancodes may be one byte long or two byte long
             * in the case where it's one byte long, the first byte is always 0xE0
             * so when we read this scancode, we can assure that this is not the full scancode
             */
            if (scancode == CODE_HEADER) {
              bytes[0] = scancode; // sets the scancode array to be the read value
              size++;              // increases the size count so we can use it as an index for the next byte
            }
            else {
              kbc_ih();               // calls the interrupt handler to read the next byte (if there's no header, the index here will be simply 0)
              bytes[size] = scancode; // sets the array to the read value
            }

            kbd_print_scancode(!(scancode & MAKE_CODE), size + 1, bytes); // calls the provided function
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
    size = 0; // resets the size variable for the next iteration
  }

  if (kbd_unsubscribe_int() != 0) { // unsubscribes the interrupt
    perror("Failed to unsubscribe kbd interrupts.");
    return 1;
  }

  if (kbd_print_no_sysinb(cnt_sys_inb) != 0) { // calls the provided function
    perror("Failed to print no_sysinb.");
    return 1;
  }

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

  if (kbd_enable_int() != 0) {
    perror("Failed to enable kbd interrupts.");
    return 1;
  }

  if (kbd_print_no_sysinb(cnt_sys_inb) != 0) {
    perror("Failed to print no_sysinb.");
    return 1;
  }

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, r, size = 0;
  uint8_t bit_no, irq_set_timer, irq_set_kbd, time = 0;
  message msg;

  if (timer_subscribe_int(&bit_no) != 0) {
    perror("Failed to subscribe timer interrupts.");
    return 1;
  }
  irq_set_timer = BIT(bit_no);

  if (kbd_subscribe_int(&bit_no) != 0) {
    perror("Failed to subscribe kbd interrupts.");
    return 1;
  }
  irq_set_kbd = BIT(bit_no);

  while (scancode != BREAK_ESC && time < n) { /* breakcode ESC and timeout check */
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 /* hardware interrupt notification */
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

  if (kbd_unsubscribe_int() != 0) {
    perror("Failed to unsubscribe kbd interrupts.");
    return 1;
  }
  if (timer_unsubscribe_int() != 0) {
    perror("Failed to unsubscribe timer interrupts.");
    return 1;
  }

  if (kbd_print_no_sysinb(cnt_sys_inb) != 0) {
    perror("Failed to print no_sysinb.");
    return 1;
  }

  return 0;
}
