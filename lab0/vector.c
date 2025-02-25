#include "vector.h"
#include <math.h>
#include <stdio.h>

Vector create_vector(double x, double y, double z) {
  Vector v = {x, y, z};
  return v;
}

Vector add_vectors(const Vector *v1, const Vector *v2) {
  return create_vector(v1->x + v2->x, v1->y + v2->y, v1->z + v2->z);
}

Vector subtract_vectors(const Vector *v1, const Vector *v2) {
  return create_vector(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
}

double dot_product(const Vector *v1, const Vector *v2) {
  return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

Vector cross_product(const Vector *v1, const Vector *v2) {
  return create_vector(
    v1->y * v2->z - v1->z * v2->y,
    v1->z * v2->x - v1->x * v2->z,
    v1->x * v2->y - v1->y * v2->x);
}

Vector scalar_multiply(const Vector *v, double scalar) {
  return create_vector(v->x * scalar, v->y * scalar, v->z * scalar);
}

double vector_magnitude(const Vector *v) {
  return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

Vector normalize_vector(const Vector *v) {
  double magnitude = vector_magnitude(v);
  return magnitude > 0 ? scalar_multiply(v, 1.0 / magnitude) : create_vector(0, 0, 0);
}

void print_vector(const Vector *v) {
  printf("(%.2f, %.2f, %.2f)\n", v->x, v->y, v->z);
}
