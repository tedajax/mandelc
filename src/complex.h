#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <math.h>
#include <assert.h>
#include <stdlib.h>

typedef struct complex_t {
	float real;
	float imaginary;
} Complex;

Complex *complex_new(float r, float i);
float complex_mag(Complex *self);
float complex_magSqr(Complex *self);
int complex_mandelbrot_iterations(Complex *self, int maxIter);
int mandelbrot_iterations(float real, float imaginary, int maxIter);

#endif