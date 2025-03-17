#include <lcom/lcf.h>

#include "kbc.h"

int(kbc_ready)(uint8_t *st) {
  return st == NULL ? 1 : !((*st) & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR));
}

int(kbc_read_st)(uint8_t *st) {
  return util_sys_inb(KBC_ST, st);
}

int(kbc_read_buffer)(uint8_t port, uint8_t *data) {
  return util_sys_inb(port, data);
}

int(kbc_read_data)(uint8_t *data) {
  uint8_t st;
  if (kbc_read_st(&st) != 0)
    return 1;

  if (st & KBC_FULL_OBF) {
    if (kbc_read_buffer(KBC_OUT, data) != 0)
      return 1;

    if (kbc_ready(&st))
      return 0;
  }

  return 1;
}

int(kbc_write_cmd)(int port, uint8_t cmd) {
  uint8_t st = 0, timeout_cnt = 0;

  while (timeout_cnt < 5) {
    if (kbc_read_st(&st) != 0)
      return 1;

    if ((st & KBC_IN) == 0) {
      if (sys_outb(port, cmd) != 0)
        return 1;

      return 0;
    }

    tickdelay(micros_to_ticks(20000));
    timeout_cnt++;
  }

  return 1;
}
