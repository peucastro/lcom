#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int w, h; // width, height
} Plane;

typedef struct {
  Plane base;
  int d; // depth
} Volume;

void print_plane(Plane *p) {
  printf("W %d, H %d\n", p->w, p->h);
}

void print_volume(Volume *v) {
  printf("W %d, H %d, D %d\n", v->base.w, v->base.h, v->d);
}

/* unions are composed of data types that store each field in the same initial
 * memory position. this means writing over a field will write over other fields
 * (only one member can store data at the given point in time) */
typedef union {
  int i;
  char c[10];
  Plane p;
} Mix;

int main() {
  Plane *plane = malloc(sizeof(Plane));
  plane->w = 3;
  plane->h = 7;

  Volume *vol = malloc(sizeof(Volume));
  vol->base = *plane;
  vol->d = 5;

  print_plane(plane);
  print_volume(vol);

  /* the union size is determined by its largest member
   * (+ any padding for proper memory alignment)
   * remember that, in 32-bit machines, memory is usually stored in multiples of 4 bytes! */
  // sizeof(int) = 4, sizeof(char) * 10 = 10, sizeof(Plane) = 4 + 4 = 8
  // the largest member is c (10) + padding (2) = 12 bytes
  printf("sizeof(Mix)=%zu\n", sizeof(Mix));

  /* Memory Layout Analysis for Mix union (12 bytes total):
   * Initial state after Mix m declaration:
   * [0-3]     [4-7]     [8-11]    // byte ranges
   * xx xx xx xx xx xx xx xx xx xx xx xx    // uninitialized memory
   */
  Mix m;

  /* After m.p.h = 3:
   * [0-3]     [4-7]     [8-11]
   * xx xx xx xx 03 00 00 00 00 00 00 00
   *             ^h=3     ^padding
   */
  m.p.h = 3;

  /* After m.p.w = 5:
   * [0-3]     [4-7]     [8-11]
   * 05 00 00 00 03 00 00 00 00 00 00 00
   * ^w=5        ^h=3     ^padding
   */
  m.p.w = 5;
  printf("m.i=%d\n", m.i); // output: m.1=5
  print_plane(&m.p);       // output: W 5, H 3 -> w and h occupy different memory positions

  /* After strcpy(m.c, "Hello!\n"):
   * [0-3]     [4-7]     [8-11]
   * 48 65 6C 6C 6F 21 0A 00 00 00 00 00
   * H  e  l  l  o  !  \n \0 ^padding
   */
  strcpy(m.c, "Hello!\n"); // overwrites the previous values of p.2 and p.h
  printf("%s", m.c);       // output: Hello!

  // outputs an ivalid value, since the memory position of m.i was already overwritten
  printf("m.i=%d\n", m.i);

  free(plane);
  free(vol);
  return 0;
}
