#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT(n) (1 << n)

unsigned int and_repl(unsigned int uint1, unsigned int uint2) {
  return uint1 & uint2;
}

unsigned int or_repl(unsigned int uint1, unsigned int uint2) {
  return uint1 | uint2;
}

unsigned int set_repl(unsigned int uint1, unsigned int uint2) {
  if (uint2 >= sizeof(unsigned int) * 8) {
    perror("please provide a valid value!\n");
    exit(EXIT_FAILURE);
  }
  return uint1 | BIT(uint2);
}

unsigned int unset_repl(unsigned int uint1, unsigned int uint2) {
  if (uint2 >= sizeof(unsigned int) * 8) {
    perror("please provide a valid value!\n");
    exit(EXIT_FAILURE);
  }
  return uint1 & ~(BIT(uint2));
}

unsigned int byte_repl(unsigned int uint1, unsigned int uint2) {
  if (uint2 >= sizeof(unsigned int)) {
    perror("please provide a valid value!\n");
    exit(EXIT_FAILURE);
  }
  return (uint1 >> (uint2 * 8)) & 0xFF;
}

int main(int argc, char **argv) {
  if (argc != 4) {
    perror("correct usage:\n    and <uint1> <uint2>\n    or <uint1> <uint2>\n    set <uint> <bitpos>\n    unset <uint> <bitpos>\n    byte <uint> <bytepos>\n");
    exit(EXIT_FAILURE);
  }

  unsigned int ans = 0;

  if (strcmp(argv[1], "and") == 0) {
    ans = and_repl((unsigned int) strtoul(argv[2], NULL, 0), (unsigned int) strtoul(argv[3], NULL, 0));
  }
  else if (strcmp(argv[1], "or") == 0) {
    ans = or_repl((unsigned int) strtoul(argv[2], NULL, 0), (unsigned int) strtoul(argv[3], NULL, 0));
  }
  else if (strcmp(argv[1], "set") == 0) {
    ans = set_repl((unsigned int) strtoul(argv[2], NULL, 0), (unsigned int) strtoul(argv[3], NULL, 0));
  }
  else if (strcmp(argv[1], "unset") == 0) {
    ans = unset_repl((unsigned int) strtoul(argv[2], NULL, 0), (unsigned int) strtoul(argv[3], NULL, 0));
  }
  else if (strcmp(argv[1], "byte") == 0) {
    ans = byte_repl((unsigned int) strtoul(argv[2], NULL, 0), (unsigned int) strtoul(argv[3], NULL, 0));
  }
  else {
    perror("correct usage:\n    and <uint1> <uint2>\n    or <uint1> <uint2>\n    set <uint> <bitpos>\n    unset <uint> <bitpos>\n    byte <uint> <bytepos>\n");
    exit(EXIT_FAILURE);
  }

  printf("%u\n", ans);

  exit(EXIT_SUCCESS);
}
