#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT(n) (1 << (n))

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

void repl() {
  char input[256];
  char command[16];
  unsigned int uint1, uint2;
  unsigned int ans = 0;
  const char *usage = "correct usage:\n"
                      "    and <uint1> <uint2>\n"
                      "    or <uint1> <uint2>\n"
                      "    set <uint> <bitpos>\n"
                      "    unset <uint> <bitpos>\n"
                      "    byte <uint> <bytepos>\n"
                      "    exit\n";

  while (1) {
    printf("> ");
    if (!fgets(input, sizeof(input), stdin)) {
      break;
    }

    if (sscanf(input, "%15s %u %u", command, &uint1, &uint2) < 1) {
      fprintf(stderr, "%s", usage);
      continue;
    }

    if (strcmp(command, "exit") == 0) {
      break;
    }
    else if (strcmp(command, "and") == 0) {
      ans = bit_and(uint1, uint2);
    }
    else if (strcmp(command, "or") == 0) {
      ans = bit_or(uint1, uint2);
    }
    else if (strcmp(command, "set") == 0) {
      ans = set_bit(uint1, uint2);
    }
    else if (strcmp(command, "unset") == 0) {
      ans = clear_bit(uint1, uint2);
    }
    else if (strcmp(command, "byte") == 0) {
      ans = get_byte(uint1, uint2);
    }
    else {
      fprintf(stderr, "%s", usage);
      continue;
    }

    printf("%u\n", ans);
  }
}

int main() {
  repl();
  return 0;
}
