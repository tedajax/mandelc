#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <math.h>
#include <assert.h>
#include <stdlib.h>

typedef struct complex_t {
	double real;
	double imaginary;
} Complex;

Complex *complex_new(double r, double i);
double complex_mag(Complex *self);
double complex_magSqr(Complex *self);
int complex_mandelbrot_iterations(Complex *self, int maxIter);
int mandelbrot_iterations(double real, double imaginary, int maxIter);

#endif