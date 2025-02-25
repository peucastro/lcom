#include <stdio.h>

void print_bits(int n) {
  // iterates from 31 to 0
  // sizeof(int) = 4 (bytes) * 8 (bits) = 32 (bits)
  // we want to print the bits from left to right
  for (int i = 31; i >= 0; i--) {
    // use the counter to shift the number
    printf("%d", (n >> i) & 1);
  }
  printf("\n");
}

int main() {
  print_bits(2);
  print_bits(3);
  print_bits(6);
  print_bits(16);
  print_bits(255);
  print_bits(-1);
  print_bits(-5);
  return 0;
}
