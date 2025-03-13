#include <lcom/lcf.h>

#include "kbc.h"

int(read_kbc_st)(uint8_t *st) {
  return util_sys_inb(KBC_ST, st);
}

int(read_kbc_data)(uint8_t *data) {
  return util_sys_inb(KBC_OUT, data);
}

int(kbc_ready)(uint8_t *st) {
  return st == NULL ? 1 : !((*st) & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR));
}
