#include <lcom/lcf.h>

#include "kbc.h"

int(kbc_ready)(uint8_t st) {
  return !((st) & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)); // checks if the parity err or timeour err bits are set to 1
}

int(kbc_read_st)(uint8_t *st) {
  if (st == NULL) {
    perror("kbc_read_st: st pointer cannot be null.");
    return 1;
  }
  if (util_sys_inb(KBC_ST, st) != 0) { // reads the value stored at the status register
    perror("kbc_read_st: failed to read the value stored at the status register.");
    return 1;
  }

  return 0;
}

int(kbc_read_data)(uint8_t *data) {
  if (data == NULL) {
    perror("kbc_read_data: data pointer cannot be null.");
    return 1;
  }

  uint8_t st = 0, attemps = 5;

  while (attemps > 0) {
    if (kbc_read_st(&st) != 0) { // reads the KBC status
      perror("kbc_read_data: failed to read the kbc status.");
      return 1;
    }

    if (st & KBC_FULL_OBF) {                  // checks if the "output buffer full" bit is set to 1
      if (util_sys_inb(KBC_OUT, data) != 0) { // effectivelly reads the value stored at the output buffer
        perror("kbc_read_data: failed to read the kbc buffer.");
        return 1;
      }
      if (!kbc_ready(st)) {
        perror("KBC not ready.");
        return 1;
      }
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attemps--;
  }

  perror("kbc_read_data: failed to read the kbc data.");
  return 1;
}

int(kbc_write_cmd)(int port, uint8_t cmd) {
  uint8_t st = 0, attempts = 5;

  /*
   * both the KBC and the keyboard may take some time to respond to a command
   * thus this "driver" should not expect to get a response immediately after issuing a command
   * we need to give enough-time for the KBC to respond, retry a few times on time-out, and finally give up
   */
  while (attempts > 0) {
    if (kbc_read_st(&st) != 0) { // reads the kbc status
      perror("kbc_write_cmd: failed to read the kbc status.");
      return 1;
    }

    if (!kbc_ready(st)) {
      perror("KBC not ready.");
      return 1;
    }

    if (!(st & KBC_FULL_IBF)) {       // checks if the input buffer is NOT alredy full
      if (sys_outb(port, cmd) != 0) { // effectivelly sends the command byte to the specified port
        perror("kbc_write_cmd: failed to read the kbc port.");
        return 1;
      }

      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US)); // waits (the cpu is way faster than the kbc)
    attempts--;                           // increases the timeout counter
  }

  perror("kbc_write_cmd: failed to write the kbc command.");
  return 1;
}
