#include <lcom/lcf.h>

#include <stdint.h>

#ifdef LAB3
int cnt_sys_inb = 0;
#endif

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL)
    return 1;

  *lsb = val & 0xFF; // bitmask to get the 8 least significant bits
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL)
    return 1;

  // first we need to shift the value to put the MSB in the desired position
  *msb = ((val >> 8) & 0xFF); // same bitmask as the last function
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL)
    return 1;

  uint32_t val; // sys_inb expects a 4 bytes variable
  int ret = sys_inb(port, &val);

  // count the number of sys_inb calls
  #ifdef LAB3
  cnt_sys_inb++;
  #endif

  /*
   * since the timer's configuration only occupies 8 bits (the status byte),
   * if we try to read it using the normal sys_inb with a uint8_t, we would get a seg fault,
   * because we are trying to acces a non-valid memory position
   */
  *value = val & 0xFF; // extract the least significant byte from the 4-byte value

  return ret;
}
