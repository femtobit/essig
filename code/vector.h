#ifndef ESSIG_VECTOR_H
#define ESSIG_VECTOR_H

typedef struct { double x[3]; }    Vector;
typedef struct { double m[3][3]; } Matrix;

/// Gibt einen Vektor z = x + y zurück
Vector vector_add(const Vector x, const Vector y);

/// Gibt einen Vektor z = x - y zurück
Vector vector_substract(const Vector x, const Vector y);

/// Gibt einen Vektor z = a mal x zurück
Vector vector_scalar_mult(const Vector x, const double a);

/// Gibt das Skalarprodukt z = x mal y zurück.
double vector_scalar_product(const Vector x, const Vector y);

/// Gibt den um den Winkel phi um die z-Achse rotierten Vektor x zurück
Vector vector_rotate_z(const Vector x, const double phi);

/// Gibt das Produkt einer 3x3 Matrix A mit einem Spaltenvektor x zurück
Vector matrix_times_vector(const Matrix A, const Vector x);

/// Gibt die zu A transponierte Matrix At zurück
Matrix matrix_transpose(const Matrix A);

#endif//ESSIG_VECTOR_H