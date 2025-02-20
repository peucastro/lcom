#include <stdio.h>

// structs -> a user-defined data type. a way to group several variables into one place
// (a named set of values that occupy a block of memory)
typedef struct {
  int w, h; // width, height
} Plane;

typedef struct {
  Plane base;
  int d; // depth
} Volume;

void print_plane(Plane *p) {
  printf("W %d, H %d\n", p->w, p->h); // p->w is equivalent of (*p).w
}

void print_volume(Volume *v) {
  // the . operator is used for struct values, while the -> operator is used for struct references
  printf("W %d, H %d, D %d\n", v->base.w, v->base.h, v->d);
}

int main() {
  Plane p = {5, 2};
  Volume v = {p, 3};
  print_plane(&p);

  /* We can print plane on a Volume because the memory is stored ordered by
   * the declaration of fields and since the first member of Volume is a Plane, by
   * casting the pointer of the Volume variable, it will be interpreted as a Plane,
   * allowing for similar abstraction of object-oriented programming. */
  print_plane((Plane *) &v);
  print_volume(&v);
  return 0;
}
