#include <lcom/lcf.h>

#include "kbc.h"

int(kbc_ready)(uint8_t st) {
  return !((st) & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)); // checks if the parity err or timeour err bits are set to 1
}

int(kbc_read_st)(uint8_t *st) {
  return util_sys_inb(KBC_ST, st); // reads the value stored at the status register
}

int(kbc_read_buffer)(uint8_t port, uint8_t *data) {
  return util_sys_inb(port, data); // reads the value stored in the indicated port
}

int(kbc_read_data)(uint8_t *data) {
  uint8_t st;
  if (kbc_read_st(&st) != 0) { // reads the KBC status
    perror("Failed to read the kbc status.");
    return 1;
  }

  if (kbc_ready(st) && (st & KBC_FULL_OBF)) {  // checks for errors and if the "output buffer full" bit is set to 1
    if (kbc_read_buffer(KBC_OUT, data) != 0) { // effectivelly reads the value stored at the output buffer
      perror("Failed to read the kbc buffer.");
      return 1;
    }

    return 0;
  }

  return 1;
}

int(kbc_write_cmd)(int port, uint8_t cmd) {
  uint8_t st = 0, timeout_cnt = 0;

  /*
   * both the KBC and the keyboard may take some time to respond to a command
   * thus this "driver" should not expect to get a response immediately after issuing a command
   * we need to give enough-time for the KBC to respond, retry a few times on time-out, and finally give up
   */
  while (timeout_cnt < 5) {
    if (kbc_read_st(&st) != 0) { // reads the kbc status
      perror("Failed to read the kbc status.");
      return 1;
    }

    if (!(st & KBC_FULL_IBF)) {       // checks if the input buffer is NOT alredy full
      if (sys_outb(port, cmd) != 0) { // effectivelly sends the command byte to the specified port
        perror("Failed to read the kbc port.");
        return 1;
      }

      return 0;
    }

    tickdelay(micros_to_ticks(20000)); // waits (the cpu is way faster than the kbc)
    timeout_cnt++;                     // increases the timeout counter
  }

  return 1;
}
