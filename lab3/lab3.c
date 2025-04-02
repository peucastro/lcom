#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"
#include "kbd.h"

static uint8_t bytes[2];
extern uint32_t cnt_sys_inb;
extern uint32_t counter;

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
  int ipc_status, r, i = 0;
  uint8_t bit_no, irq_set;
  message msg;

  if (kbd_subscribe_int(&bit_no) != 0) { // subscribes for the kbd interrupts
    perror("Failed to subscribe kbd interrupts.");
    return 1;
  }
  irq_set = BIT(bit_no); // create a bitmask to "filter" the interrupt messages

  while (get_scancode() != BREAK_ESC) { // use getter to access scancode
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih();                              // Calls the interrupt handler once
            bytes[i] = get_scancode();             // stores the scancode (or the second scancode, in the case where the first one was a header)

            if (get_scancode() == CODE_HEADER) { // check if it's a header
              i++;                               // increase index for the next byte
              continue;
            }

            kbd_print_scancode(!(get_scancode() & MAKE_CODE), i + 1, bytes); // prints the scancode
            i = 0;                                                           // resets the index for the next iteration
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (kbd_unsubscribe_int() != 0) { // unsubscribes the kbd interrupt
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
  int i = 0;    // index used for handling scancondes with 2 bytes
  uint8_t data; // variable to store the data read from the KBC

  while (get_scancode() != BREAK_ESC) { // loop until the ESC breakcode is detected
    if (kbc_read_data(&data) == 0) {    // reads data from the KBC output buffer
      set_scancode(data);               // sets the scancode using the setter function
      bytes[i] = get_scancode();        // stores the scancode (or the second scancode, in the case where the first one was a header)

      if (get_scancode() == CODE_HEADER) { // checks if the scancode is a header (indicating a two-byte scancode)
        i++;                               // increments the size to prepare for the next byte
        continue;
      }

      kbc_read_data(&data); // reads the next byte of the scancode
      set_scancode(data);   // sets the scancode using the setter function

      kbd_print_scancode(!(get_scancode() & MAKE_CODE), i + 1, bytes); // prints the scancode using the provided function
      i = 0;                                                           // increase index for the next byte
    }
    tickdelay(micros_to_ticks(DELAY_US)); // delay to prevent excessive polling
  }

  if (kbd_enable_int() != 0) { // re-enables kbd interrupts after polling
    perror("Failed to enable kbd interrupts.");
    return 1;
  }

  if (kbd_print_no_sysinb(cnt_sys_inb) != 0) { // calls the provided function
    perror("Failed to print no_sysinb.");
    return 1;
  }

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, r, i = 0;
  uint8_t bit_no, irq_set_timer, irq_set_kbd, time = 0;
  message msg;

  if (timer_subscribe_int(&bit_no) != 0) { // subscribes for the timer interrupts
    perror("Failed to subscribe timer interrupts.");
    return 1;
  }
  irq_set_timer = BIT(bit_no);

  if (kbd_subscribe_int(&bit_no) != 0) { // subscribes for the kbd interrupts
    perror("Failed to subscribe kbd interrupts.");
    return 1;
  }
  irq_set_kbd = BIT(bit_no);

  while (get_scancode() != BREAK_ESC && time < n) { /* breakcode ESC and timeout check */
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (counter % 60 == 0) {
              time++;
            }
          }
          if (msg.m_notify.interrupts & irq_set_kbd) { /* subscribed interrupt */
            kbc_ih();                                  // Calls the interrupt handler once
            bytes[i] = get_scancode();                 // stores the scancode (or the second scancode, in the case where the first one was a header)

            if (get_scancode() == CODE_HEADER) { // check if it's a header
              i++;                               // increase index for the next byte
              continue;
            }

            kbd_print_scancode(!(get_scancode() & MAKE_CODE), i + 1, bytes); // prints the scancode
            time = 0;
            counter = 0; // clear out the timer's counter
            i = 0;       // resets the index for the next iteration
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (timer_unsubscribe_int() != 0) { // unsubscribes the timer interrupt
    perror("Failed to unsubscribe timer interrupts.");
    return 1;
  }
  if (kbd_unsubscribe_int() != 0) { // unsubscribes the kbd interrupt
    perror("Failed to unsubscribe kbd interrupts.");
    return 1;
  }

  if (kbd_print_no_sysinb(cnt_sys_inb) != 0) { // calls the provided function
    perror("Failed to print no_sysinb.");
    return 1;
  }

  return 0;
}
