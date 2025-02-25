#include <stdio.h>

#define BIT(n) (1 << (n)) // () prevent precedence issues

int main() {
  // bitwise operators
  // & (and), | (or), ^ (xor), ~ (not), << (left shift), >> (right shift)

  int n = 0; // n = 0b0000
  printf("n=%d\n", n);
  n = n | 1; // n = 0b0000 | 0b0001 = 0b0001 (1)
  printf("n|1=%d\n", n);

  // explanation:
  // 0 1 0 1 (5)
  // 0 0 1 0 (2)
  // ------- xor
  // 0 1 1 1 (7)
  printf("0b101^0b10=%d\n", 0b101 ^ 0b10);

  // explanation:
  //   1 1 1 1 (15)
  //   0 0 0 1 (1)
  //   ------- sum
  // 1 0 0 0 0 (16)
  printf("0xF+1=%d\n", 0xF + 1);

  // masks -> a trick to manipulate individual bits
  int k = 0xE;
  printf("k=%d\n", k);

  // reading the value of bit 3: (we start counting from 0)
  // k = 0b1110
  // k >> 3 = 0b0001
  //
  // 0 0 0 1
  // 0 0 0 1
  // ------- and
  // 0 0 0 1
  printf("Bit 3 of k is %d\n", (k >> 3) & 1);

  // setting the value of bit 4:
  // 1 << 4 = 0b10000
  //
  // 0 1 1 1 0
  // 1 0 0 0 0
  // --------- or
  // 1 1 1 1 0
  k |= (1 << 4); // k = k | (1 << 4)
  printf("k=%d\n", k);

  // clearing the bit 3:
  // 1 << 3 = 0b01000
  // ~(1 << 3) = 0b10111
  //
  // 1 1 1 1 0
  // 1 0 1 1 1
  // --------- and
  // 1 0 1 1 0
  k &= ~(1 << 3); // k = k & ~(1 << 3)
  printf("k=%d\n", k);

  // using macros
  k |= BIT(3);    // set bit 3
  k &= ~BIT(3);   // clear bit 3 (always use the negation for clearing)
  if (k & BIT(2)) // check if bit 2 is set
    printf("bit 2 is set!\n");

  return 0;
}
