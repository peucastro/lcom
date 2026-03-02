#include "bitwise.h"

uint8_t clear(uint8_t msk, int pos) {
  return msk & ~(1 << pos);
}

uint8_t set(uint8_t msk, int pos) {
  return msk | (1 << pos);
}

bool is_set(uint8_t msk, int pos) {
  return (msk & (1 << pos)) != 0;
}

uint8_t lsb(uint16_t wide_msk) {
  return wide_msk & 0xFF;
}

uint8_t msb(uint16_t wide_msk) {
  return (wide_msk >> 8) & 0xFF;
}

uint8_t mask(int pos, ...) {
  uint8_t res = 0;
  va_list args;

  va_start(args, pos);

  int current = pos;
  while (current != MSK_END) {
    res |= (1 << current);
    current = va_arg(args, int);
  }

  va_end(args);
  return res;
}
