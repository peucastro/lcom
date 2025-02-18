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
  // & -> get memory address of a variable
  // * -> get value stored at a memory address
  int n = 5;
  int *p = &n;
  printf("n=%d\n", n); // %d - format to a decimal
  printf("*p=%d\n", *p);

  // print the memory position of p:
  printf("p=%u\n", p); // %u - format to an unsigned int

  // pointers of pointers -> variables which value is the memory location of a pointer
  int **pp = &p;
  printf("*pp=%u\n", *pp); // equivalent to the last print because p and *pp both hold the same address

  // pointer casting
  *p = 0;
  char *cp = (char *) p;
  cp[0] = 'A';
  printf("*cp=%c\n", *cp);
  printf("*p=%d\n", *p);

  // disclaimer: this will output 4, as minix is a 32 bit machine.
  // if you compile this and run it at your host machine, it will output 8
  printf("sizeof(int) = %lu\n", sizeof(int));

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

  printf("sizeof(s)=%u\n", sizeof(s)); // outputs 400, allocated at compile time
  printf("sizeof(d)=%u\n", sizeof(d)); // outputs 4, the size of the pointer

  print_array(s, 5); // s and d are both pointers to the first position
  print_array(d, 5); // of their respective array

  free(d);
  return 0;
}
