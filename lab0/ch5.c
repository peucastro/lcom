#include "vector.h"
#include <stdio.h>

// to compile this program, use the Makefile included
int main() {
  Vector v1 = create_vector(1.0, 2.0, 3.0);
  Vector v2 = create_vector(4.0, 5.0, 6.0);

  Vector sum = add_vectors(&v1, &v2);
  Vector diff = subtract_vectors(&v1, &v2);
  double product = dot_product(&v1, &v2);
  Vector cross = cross_product(&v1, &v2);
  Vector scaled = scalar_multiply(&v1, 2.0);
  double magnitude = vector_magnitude(&v1);
  Vector normalized = normalize_vector(&v1);

  printf("vector 1: ");
  print_vector(&v1);
  printf("vector 2: ");
  print_vector(&v2);

  printf("sum: ");
  print_vector(&sum);

  printf("difference: ");
  print_vector(&diff);

  printf("dot product: %.2f\n", product);

  printf("cross product: ");
  print_vector(&cross);

  printf("scaled vector 1 (2x): ");
  print_vector(&scaled);

  printf("magnitude of vector 1: %.2f\n", magnitude);

  printf("normalized vector 1: ");
  print_vector(&normalized);

  return 0;
}
