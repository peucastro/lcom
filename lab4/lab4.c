#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"

extern uint32_t counter;

typedef enum {
  START,
  RIGHT_UP,
  VERTEX,
  RIGHT_DOWN,
  END
} drawing_state;

void(update_drawing_state)(drawing_state *state, struct mouse_ev *ev, uint8_t tolerance, uint8_t x_len, int16_t *x_delta, int16_t *y_delta) {
  switch (*state) {
    case START:
      *x_delta = 0;
      *y_delta = 0;
      if (ev->type == LB_PRESSED) {
        *state = RIGHT_UP;
      }
      break;

    case RIGHT_UP:
      if (ev->type == LB_RELEASED) {
        if ((*x_delta >= x_len) && (*y_delta / *x_delta) >= 1) {
          *x_delta = 0;
          *y_delta = 0;
          *state = VERTEX;
        }
        else {
          *state = START;
        }
      }
      else if (ev->type == MOUSE_MOV) {
        if (ev->delta_x <= 0 || ev->delta_y <= 0) {
          if (abs(ev->delta_x) > tolerance || abs(ev->delta_y) > tolerance) {
            *state = START;
            break;
          }
          if (ev->delta_x != 0 && (ev->delta_y) / (ev->delta_x) <= 1) {
            *state = START;
          }
        }
        else {
          *x_delta += ev->delta_x;
          *y_delta += ev->delta_y;
        }
      }
      else {
        *state = START;
      }
      break;

    case VERTEX:
      if (ev->type == RB_PRESSED) {
        if (abs(*x_delta) > tolerance || abs(*y_delta) > tolerance) {
          *state = START;
        }
        else {
          *x_delta = 0;
          *y_delta = 0;
          *state = RIGHT_DOWN;
        }
      }
      else if (ev->type == MOUSE_MOV) {
        *x_delta += ev->delta_x;
        *y_delta += ev->delta_y;
      }
      else if (ev->type == LB_PRESSED) {
        *x_delta = 0;
        *y_delta = 0;
        *state = RIGHT_UP;
      }
      else {
        *state = START;
      }
      break;

    case RIGHT_DOWN:
      if (ev->type == RB_RELEASED) {
        if ((*x_delta >= x_len) && (*y_delta / *x_delta) <= -1) {
          *state = END;
          return;
        }
        else
          *state = START;
      }
      else if (ev->type == MOUSE_MOV) {
        if (ev->delta_x <= 0 || ev->delta_y >= 0) {
          if (abs(ev->delta_x) > tolerance || abs(ev->delta_y) > tolerance) {
            *state = START;
            break;
          }
          if (ev->delta_x != 0 && (ev->delta_y) / (ev->delta_x) >= -1) {
            *state = START;
          }
        }
        else {
          *x_delta += ev->delta_x;
          *y_delta += ev->delta_y;
        }
      }
      else {
        *state = START;
      }
      break;

    case END:
      break;

    default:
      *state = START;
      break;
  }
}

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  int ipc_status, r;
  uint8_t bit_no, irq_set;
  message msg;

  struct packet pp;

  if (mouse_write_cmd(MOUSE_EN_DATA_REPORTS) != 0) {
    return 1;
  }

  if (mouse_subscribe_int(&bit_no) != 0) {
    return 1;
  }
  irq_set = BIT(bit_no);

  while (cnt) {
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            mouse_ih();                            // calls the interrupt handler
            mouse_sync();                          // sync the mouse index

            /* when the index is reset to 0, it indicates that a complete
             * mouse packet has been successfully read. at this point,
             * the packet should be processed and printed. */
            if (mouse_get_index() == 0) {
              pp = mouse_parse_packet();
              mouse_print_packet(&pp);
              cnt--; // decrease the number of packets parsed
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
  }

  if (mouse_unsubscribe_int() != 0) {
    return 1;
  }

  if (mouse_write_cmd(MOUSE_DIS_DATA_REPORTS) != 0) {
    return 1;
  }

  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  int ipc_status, r;
  uint8_t bit_no, irq_set_timer, irq_set_mouse;
  message msg;

  uint8_t time = 0;
  struct packet pp;

  if (timer_subscribe_int(&bit_no) != 0) {
    return 1;
  }
  irq_set_timer = BIT(bit_no);

  if (mouse_write_cmd(MOUSE_EN_DATA_REPORTS) != 0) {
    return 1;
  }

  if (mouse_subscribe_int(&bit_no) != 0) {
    return 1;
  }
  irq_set_mouse = BIT(bit_no);

  while (time < idle_time) {
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
            if (counter % sys_hz() == 0) {
              time++;
            }
          }
          if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */
            mouse_ih();
            mouse_sync();

            if (mouse_get_index() == 0) {
              pp = mouse_parse_packet();
              mouse_print_packet(&pp);
              // reset the timer couter and our program counter
              time = 0;
              counter = 0;
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
  }

  if (mouse_unsubscribe_int() != 0) {
    return 1;
  }

  if (mouse_write_cmd(MOUSE_DIS_DATA_REPORTS) != 0) {
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status, r;
  uint8_t bit_no, irq_set;
  message msg;

  struct packet pp;
  struct mouse_ev ev;
  drawing_state state = START;
  int16_t x_delta, y_delta = 0;

  if (mouse_write_cmd(MOUSE_EN_DATA_REPORTS) != 0) {
    return 1;
  }

  if (mouse_subscribe_int(&bit_no) != 0) {
    return 1;
  }
  irq_set = BIT(bit_no);

  // loop while we're not at the desired state
  while (state != END) {
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            mouse_ih();
            mouse_sync();

            if (mouse_get_index() == 0) {
              pp = mouse_parse_packet();
              mouse_print_packet(&pp);
              ev = *mouse_detect_event(&pp); // use the provided function to detect the mouse event
              update_drawing_state(&state, &ev, tolerance, x_len, &x_delta, &y_delta);
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
  }

  if (mouse_unsubscribe_int() != 0) {
    return 1;
  }

  if (mouse_write_cmd(MOUSE_DIS_DATA_REPORTS) != 0) {
    return 1;
  }

  return 0;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* This year you need not implement this. */
  printf("%s(%u, %u): under construction\n", __func__, period, cnt);
  return 1;
}
