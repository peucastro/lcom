#include <lcom/lcf.h>

#include "i8042.h"
#include <lcom/lab3.h>

int(read_kbc_st)(uint8_t *st) {
  return util_sys_inb(KBC_ST, st);
}

int (read_kbc_data)(uint8_t *data) {
  return util_sys_inb(KBC_OUT, data);
}
