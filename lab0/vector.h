#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
  double x;
  double y;
  double z;
} Vector;

Vector create_vector(double x, double y, double z);
Vector add_vectors(const Vector *v1, const Vector *v2);
Vector subtract_vectors(const Vector *v1, const Vector *v2);
double dot_product(const Vector *v1, const Vector *v2);
Vector cross_product(const Vector *v1, const Vector *v2);
Vector scalar_multiply(const Vector *v, double scalar);
double vector_magnitude(const Vector *v);
Vector normalize_vector(const Vector *v);
void print_vector(const Vector *v);

#endif
