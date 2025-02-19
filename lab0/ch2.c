#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT(n) (1 << n)

// performs bitwise and of two unsigned ints
unsigned int bit_and(unsigned int uint1, unsigned int uint2) {
  return uint1 & uint2;
}

// performs bitwise or of two unsigned ints
unsigned int bit_or(unsigned int uint1, unsigned int uint2) {
  return uint1 | uint2;
}

// sets the bit at position 'bitpos' in uint1
unsigned int set_bit(unsigned int uint1, unsigned int bitpos) {
  if (bitpos >= sizeof(unsigned int) * 8) {
    fprintf(stderr, "error: please provide a valid bit position!\n");
    exit(EXIT_FAILURE);
  }
  return uint1 | BIT(bitpos);
}

// clears the bit at position 'bitpos' in uint1
unsigned int clear_bit(unsigned int uint1, unsigned int bitpos) {
  if (bitpos >= sizeof(unsigned int) * 8) {
    fprintf(stderr, "error: please provide a valid bit position!\n");
    exit(EXIT_FAILURE);
  }
  return uint1 & ~(BIT(bitpos));
}

// extracts a specific byte from uint1 based on bytepos
unsigned int get_byte(unsigned int uint1, unsigned int bytepos) {
  if (bytepos >= sizeof(unsigned int)) {
    fprintf(stderr, "error: please provide a valid byte position!\n");
    exit(EXIT_FAILURE);
  }
  return (uint1 >> (bytepos * 8)) & 0xFF; // shift and mask to extract the byte
}

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "correct usage:\n    and <uint1> <uint2>\n    or <uint1> <uint2>\n    set <uint> <bitpos>\n    unset <uint> <bitpos>\n    byte <uint> <bytepos>\n");
    exit(EXIT_FAILURE); // exit if incorrect number of arguments
  }

  char *endptr;
  // convert first numeric argument from string to unsigned int
  unsigned int uint1 = (unsigned int) strtoul(argv[2], &endptr, 0);
  if (*endptr != '\0') {
    fprintf(stderr, "invalid number: %s\n", argv[2]); // check conversion error
    exit(EXIT_FAILURE);
  }
  // convert second numeric argument from string to unsigned int
  unsigned int uint2 = (unsigned int) strtoul(argv[3], &endptr, 0);
  if (*endptr != '\0') {
    fprintf(stderr, "invalid number: %s\n", argv[3]); // check conversion error
    exit(EXIT_FAILURE);
  }

  unsigned int ans = 0;

  // decide which operation to perform based on the first argument
  if (strcmp(argv[1], "and") == 0) {
    ans = bit_and(uint1, uint2);
  }
  else if (strcmp(argv[1], "or") == 0) {
    ans = bit_or(uint1, uint2);
  }
  else if (strcmp(argv[1], "set") == 0) {
    ans = set_bit(uint1, uint2);
  }
  else if (strcmp(argv[1], "unset") == 0) {
    ans = clear_bit(uint1, uint2);
  }
  else if (strcmp(argv[1], "byte") == 0) {
    ans = get_byte(uint1, uint2);
  }
  else {
    fprintf(stderr, "correct usage:\n    and <uint1> <uint2>\n    or <uint1> <uint2>\n    set <uint> <bitpos>\n    unset <uint> <bitpos>\n    byte <uint> <bytepos>\n");
    exit(EXIT_FAILURE);
  }

  printf("%u\n", ans);
  return 0;
}
