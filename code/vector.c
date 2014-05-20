/**
 * Here universal Vector functions
 * 
 * Author(s):
 *   Michael Hufschmidt <michael@hufschmidt-web.de>
 */

#include "vector.h"

/// Gibt einen Vektor z = x + y zurück
Vector vector_add(const Vector x, const Vector y)
{
  Vector z = {{0, 0, 0}};
  z.x[0] = x.x[0] + y.x[0];
  z.x[1] = x.x[1] + y.x[1];
  z.x[2] = x.x[2] + y.x[2];
  return z;
}

/// Gibt einen Vektor z = x - y zurück
Vector vector_substract(const Vector x, const Vector y)
{
  Vector z = {{0, 0, 0}};
  z.x[0] = x.x[0] - y.x[0];
  z.x[1] = x.x[1] - y.x[1];
  z.x[2] = x.x[2] - y.x[2];
  return z;
}

/// Gibt einen Vektor z = a mal x zurück
Vector vector_scalar_mult(const Vector x, const double a)
{
  Vector z = {{0, 0, 0}};
  z.x[0] = a * x.x[1];
  z.x[1] = a * x.x[2];
  z.x[2] = a * x.x[3];
  return z;
}

/// Gibt das Skalarprodukt z = x mal y zurück.
double vector_scalar_product(const Vector x, const Vector y)
{
  return x.x[0] * y.x[0] + x.x[1] * y.x[1] + x.x[2] * y.x[2];
}

/// Gibt den um den Winkel phi um die z-Achse rotierten Vektor x zurück
Vector vector_rotate_z(const Vector x, const double phi)
{
  Vector z = {{0, 0, 0}};
}

/// Gibt das Produkt einer 3x3 Matrix A mit einem Spaltenvektor x zurück
Vector matrix_times_vector(const Matrix A, const Vector x)
{
  Vector z = {{0, 0, 0}};
}

/// Gibt die zu A transponierte Matrix At zurück
Matrix matrix_transpose(const Matrix A)
{
}
