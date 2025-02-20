#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int w, h; // width, height
} Plane;

typedef struct {
  Plane base;
  int d; // depth
} Volume;

/* the main problem of this code is that, since the plane variable was allocated statically
 * (i.e. we did not used the malloc function, so it will be allocated at the stack, not the heap),
 * it will be erased from the memory once it gets out of the scope (this is often called a dangling pointer) */
// when compiling it with the -Wall flag, we can see the following warning:
// address of stack memory associated with local variable 'plane' returned [-Wreturn-stack-address]
// although the code may work sometimes without errors (in the case where the memory wasn't overwritten yet),this is not safe and should not be reproduced
Plane *create_plane() {
  Plane plane = {1, 2};
  return &plane; // WRONG: returns pointer to stack memory
}

// here's the correct way to write this constructor, using heap allocation:
/* this function dynamically allocates memory for a Plane struct on the heap
 * and initializes its width and height.
 * the caller is responsible for freeing the allocated memory */
Plane *new_plane(int w, int h) {
  Plane *p = malloc(sizeof(Plane));
  p->w = w;
  p->h = h;
  return p;
}

/* this function dynamically allocates memory for a Volume struct on the heap
 * and initializes its base Plane and depth.
 * the caller is responsible for freeing the allocated memory */
Volume *new_volume(Plane *p, int d) {
  Volume *v = malloc(sizeof(Volume));
  v->base.w = p->w;
  v->base.h = p->h;
  v->d = d;
  return v;
}

int main() {
  Plane *plane = new_plane(1, 2);
  printf("Width is %d\n", plane->w);

  Volume *vol = new_volume(plane, 3);
  printf("Volume dimensions: %dx%dx%d\n",
         vol->base.w, vol->base.h, vol->d);

  free(plane);
  free(vol);
  return 0;
}
