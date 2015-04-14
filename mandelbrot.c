#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mandelbrot.h"

#define MIN_ITERATIONS 1
#define MAX_ITERATIONS 256
#define ESCAPE_DISTANCE 2

typedef struct _complex {
    double real;
    double imaginary;
} complex;

static int isBounded(complex num);
static double distanceFromOrigin(complex num);
static complex nextTerm(complex num, complex initial);
static complex complexSquare(complex num);
static double square(double x);

int escapeSteps(double x, double y) {
    complex initialValue = {x, y};
    complex workingCopy = initialValue;
    int iterations = MIN_ITERATIONS; // Mandelbrot assumes this

    while (isBounded(workingCopy) && iterations < MAX_ITERATIONS) {
        workingCopy = nextTerm(workingCopy, initialValue);
        iterations++;
    }
    return iterations;
}

static int isBounded(complex num) {
    int bounded = 1;
    if (distanceFromOrigin(num) >= ESCAPE_DISTANCE) {
        bounded = 0;
    }
    return bounded;
}

static double distanceFromOrigin(complex num) {
    // Distance formula
    return sqrt(square(num.real) + square(num.imaginary));
}

static complex nextTerm(complex num, complex initial) {
    num = complexSquare(num);
    num.real += initial.real;
    num.imaginary += initial.imaginary;
    return num;
}

static complex complexSquare(complex num) {
    // The square of a complex number resolves to:
    // (x + yi)**2 = (x**2 - y**2) + (2xy)i
    complex temp = num;
    num.real = square(temp.real) - square(temp.imaginary);
    num.imaginary = 2 * temp.real * temp.imaginary;
    return num;
}

static double square(double x) {
    return pow(x, 2);
}
