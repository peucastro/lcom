#include <lcom/lcf.h>

#include "kbc.h"

int(read_kbc_st)(uint8_t *st) {
  return util_sys_inb(KBC_ST, st);
}

int(read_kbc_data)(uint8_t port, uint8_t *data) {
  uint8_t st;
  if (read_kbc_st(&st) != 0)
    return 1;

  if (!kbc_ready(&st))
    return 1;

  return util_sys_inb(port, data);
}

int(kbc_ready)(uint8_t *st) {
  return st == NULL ? 1 : !((*st) & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR));
}
