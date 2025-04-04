#include <lcom/lcf.h>

#include "kbd.h"

static int kbd_hook_id = 5;
static uint8_t scancode = 0; // static global variable for communicating with our program

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    perror("kbd_subscribe_int: bit_no pointer cannot be null.");
    return 1;
  }

  *bit_no = kbd_hook_id; // saves the hook_id value so we can use it as a mask with the msg.m_notify.interrupts

  /*
   * this is very similar to the last lab, the only difference being that
   * the subscription should specify not only the IRQ_REENABLE policy but also the IRQ_EXCLUSIVE policy
   * this is done to avoid minix interrup handler to "steal" the scancodes
   */
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != 0) {
    perror("kbd_subscribe_int: failed to set the kbd interrupt subscription policy.");
    return 1;
  }

  return 0;
}

int(kbd_unsubscribe_int)(void) {
  if (sys_irqrmpolicy(&kbd_hook_id) != 0) { // unsubscribes the notification
    perror("kbd_unsubscribe_int: failed to remove the keyboard interrupt subscription policy.");
    return 1;
  }

  return 0;
}

int(kbd_enable_int)(void) {
  uint8_t cmd;
  if (kbc_write_cmd(KBC_IN, KBC_READ_CMD) != 0) { // "tells" the KBC we want to read it's command byte
    perror("kbd_enable_int: failed to write the KBC_READ_CMD.");
    return 1;
  }
  if (kbc_read_data(&cmd) != 0) { // effectivelly reads the command byte
    perror("kbd_enable_int: failed to read the kbc command.");
    return 1;
  }

  cmd |= KBC_INT; // masks the cmd with the kbd interrupt bit

  if (kbc_write_cmd(KBC_IN, KBC_WRITE_CMD) != 0) { //"tells" the KBC we want to change it's command byte
    perror("kbd_enable_int: failed to write the KBC_WRITE_CMD.");
    return 1;
  }
  if (kbc_write_cmd(KBC_ARG, cmd) != 0) { // effectivelly writes the command byte
    perror("kbd_enable_int: failed to pass the kbc argument.");
    return 1;
  }

  return 0;
}

void(kbc_ih)(void) {
  kbc_read_data(&scancode); // reads the value stored on the output buffer
}

uint8_t(get_scancode)(void) {
  return scancode; // getter function to access the scancode
}

void(set_scancode)(uint8_t value) {
  scancode = value; // setter function to modify the scancode
}
