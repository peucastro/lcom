#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_array(int *p, int n) {
  for (int i = 0; i < n; i++)
    printf("%d ", p[i]);
  printf("\n");
}

int main() {
  // pointers
  // & -> get memory address of a variable (address-of operator)
  // * -> get value stored at a memory address (dereference operator)
  int n = 5;
  int *p = &n;
  printf("n=%d\n", n); // %d - format to a decimal
  printf("*p=%d\n", *p);

  // print the memory position of p:
  printf("p=%p\n", (void *) p); // %p - format to a pointer

  // pointers of pointers -> variables which value is the memory location of a pointer
  int **pp = &p;
  printf("*pp=%p\n", (void *) *pp); // equivalent to the last print because p and *pp both hold the same address

  // pointer casting
  *p = 0;
  char *cp = (char *) p;
  cp[0] = 'A';
  printf("*cp=%c\n", *cp);
  printf("*p=%d\n", *p);

  // results: 4 and 1
  printf("sizeof(int) = %zu\n", sizeof(int));   // typically 4 bytes on most systems
  printf("sizeof(char) = %zu\n", sizeof(char)); // always 1 byte by definition

  // results: 4 and 4 (on 32-bit systems)
  // results: 8 and 8 (on 64-bit systems)
  // explanation: all pointers have the same size on a given system, as they store memory addresses
  printf("sizeof(int *) = %zu\n", sizeof(int *));
  printf("sizeof(char *) = %zu\n", sizeof(char *));

  // arrays
  int s[100] = {0};                   // all zeroes by the compiler
  int *d = malloc(100 * sizeof(int)); // new int[100] in C++
  memset(d, 0, 100);                  // all zeroes in run time

  s[1] = 1;
  d[1] = 1;
  printf("s[0]=%d\n", s[0]);       // 0
  printf("*s=%d\n", *s);           // 0 (equivalent to the last print)
  printf("s[1]=%d\n", s[1]);       // 1
  printf("*(s+1)=%d\n", *(s + 1)); // 1 (equivalent to the last print)
                                   // pointer arithmetic -> automatically adjusts the offset depending on the base type)

  printf("sizeof(s)=%zu\n", sizeof(s)); // outputs 400, allocated at compile time
  printf("sizeof(d)=%zu\n", sizeof(d)); // outputs the size of the pointer

  print_array(s, 5); // s and d are both pointers to the first position
  print_array(d, 5); // of their respective array

  free(d);
  return 0;
}
