#include "complex.h"

Complex *complex_new(double r, double i) {
	Complex *self = (Complex *)malloc(sizeof(Complex));

	self->real = r;
	self->imaginary = i;

	return self;
}

double complex_mag(Complex *self) {
	assert(self);

	return sqrt(complex_magSqr(self));
}

double complex_magSqr(Complex *self) {
	assert(self);

	return pow(self->real, 2) + pow(self->imaginary, 2);
}

int complex_mandelbrot_iterations(Complex *self, int maxIter) {
	assert(self);

	Complex z;
	z.real = 0;
	z.imaginary = 0;

	for (int i = 0; i < maxIter; ++i) {
		double tempReal = z.real;
		z.real = (z.real * z.real) - (z.imaginary * z.imaginary);
		z.imaginary *= 2 * tempReal;
		z.real += self->real;
		z.imaginary += self->imaginary;

		if (complex_magSqr(&z) >= 4) {
			return i;
		}
	}

	return maxIter - 1;
}

int mandelbrot_iterations(double real, double imaginary, int maxIter) {
	Complex z;
	z.real = 0;
	z.imaginary = 0;

	for (int i = 0; i < maxIter; ++i) {
		double tempReal = z.real;
		z.real = (z.real * z.real) - (z.imaginary * z.imaginary);
		z.imaginary *= 2 * tempReal;
		z.real += real;
		z.imaginary += imaginary;

		if (complex_magSqr(&z) >= 4) {
			return i;
		}
	}

	return maxIter - 1;	
}