#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "kbd.h"

extern uint32_t counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  // switch the video adapter to the graphics mode specified
  if (vbe_set_video_mode(mode) != 0) {
    return 1;
  }

  // waits...
  sleep(delay);

  // go back to Minix's default text mode
  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  // map the video memory to the process' address space
  if (vbe_map_vram(mode) != 0) {
    return 1;
  }

  // switch the video adapter to the graphics mode specified
  if (vbe_set_video_mode(mode) != 0) {
    return 1;
  }

  // draws a rectangle
  if (graphics_draw_rectangle(x, y, width, height, color) != 0) {
    return 1;
  }

  int ipc_status, r;
  uint8_t bit_no, irq_set;
  message msg;

  if (kbd_subscribe_int(&bit_no) != 0) { // subscribes for the kbd interrupts
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
            kbc_ih();                              // calls the interrupt handler once
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
    return 1;
  }

  // go back to Minix's default text mode
  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  // map the video memory to the process' address space
  if (vbe_map_vram(mode) != 0) {
    return 1;
  }

  // switch the video adapter to the graphics mode specified
  if (vbe_set_video_mode(mode) != 0) {
    return 1;
  }

  // draws a matrix
  if (graphics_draw_matrix(mode, no_rectangles, first, step) != 0) {
    return 1;
  }

  int ipc_status, r;
  uint8_t bit_no, irq_set;
  message msg;

  if (kbd_subscribe_int(&bit_no) != 0) { // subscribes for the kbd interrupts
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
            kbc_ih();                              // calls the interrupt handler once
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
    return 1;
  }

  // go back to Minix's default text mode
  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  // map the video memory to the process' address space
  if (vbe_map_vram(VBE_MODE_1024x768) != 0) {
    return 1;
  }

  // switch the video adapter to the graphics mode specified
  if (vbe_set_video_mode(VBE_MODE_1024x768) != 0) {
    return 1;
  }

  // draws a xpm image
  if (graphics_draw_xpm(xpm, x, y) != 0) {
    return 1;
  }

  int ipc_status, r;
  uint8_t bit_no, irq_set;
  message msg;

  if (kbd_subscribe_int(&bit_no) != 0) { // subscribes for the kbd interrupts
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
            kbc_ih();                              // calls the interrupt handler once
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
    return 1;
  }

  // go back to Minix's default text mode
  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  // map the video memory to the process' address space
  if (vbe_map_vram(VBE_MODE_1024x768) != 0) {
    return 1;
  }

  // switch the video adapter to the graphics mode specified
  if (vbe_set_video_mode(VBE_MODE_1024x768) != 0) {
    return 1;
  }

  // draws a xpm image
  if (graphics_draw_xpm(xpm, xi, yi) != 0) {
    return 1;
  }

  int ipc_status, r;
  uint8_t bit_no, irq_set_timer, irq_set_kbd;
  message msg;

  if (timer_subscribe_int(&bit_no) != 0) { // subscribes for the timer interrupts
    return 1;
  }
  irq_set_timer = BIT(bit_no); // create a bitmask to "filter" the interrupt messages

  if (kbd_subscribe_int(&bit_no) != 0) { // subscribes for the kbd interrupts
    return 1;
  }
  irq_set_kbd = BIT(bit_no); // create a bitmask to "filter" the interrupt messages

  // tells if the movement is either horizontal or vertical
  bool horizontal = (yi == yf);
  // aux variable to count how many frames are left in the case where the speed is negative
  uint8_t frames_left = -speed;

  while ((get_scancode() != BREAK_ESC)) {
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                   /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_timer) { /* timer interrupt */
            timer_int_handler();

            /* skip current iteration if:
             * 1. target position (xf, yf) has been reached, or
             * 2. current frame doesn't align with the specified frame rate
             * (60 Hz system timer / user-defined frame rate) */
            if (((xi == xf) && (yi == yf)) || (counter % (60 / fr_rate) != 0)) {
              continue;
            }

            if (speed > 0) {
              if (horizontal) {
                xi += speed;
                if (xi > xf) {
                  xi = xf;
                }
              }
              else {
                yi += speed;
                if (yi > yf) {
                  yi = yf;
                }
              }
            }
            else {
              frames_left--;
              if (frames_left == 0) {
                if (horizontal) {
                  xi += 1;
                }
                else {
                  yi += 1;
                }
                frames_left = -speed;
              }
              else {
                continue;
              }
            }

            // clears the screen
            if (graphics_clear_screen() != 0) {
              return 1;
            }
            // draws the xpm image at the updated position
            if (graphics_draw_xpm(xpm, xi, yi) != 0) {
              return 1;
            }
          }
          if (msg.m_notify.interrupts & irq_set_kbd) { /* kbd interrupt */
            kbc_ih();                                  // calls the interrupt handler once
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
    return 1;
  }

  if (kbd_unsubscribe_int() != 0) { // unsubscribes the kbd interrupt
    return 1;
  }

  // go back to Minix's default text mode
  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
